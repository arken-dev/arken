-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Lock       = require "arken.concurrent.Lock"
local HttpClient = require "arken.net.HttpClient"
local Task       = require "arken.concurrent.task"
local json       = require "arken.json"
local template   = require "arken.template"
local mvm        = require "arken.mvm"

local broker = {}

broker.storagePath = "topics"
broker.configPath  = "config/broker.json"

local config

-------------------------------------------------------------------------------
-- caminho da própria pasta (pra achar o arquivo irmão replicate_task.lua
-- independente de onde o processo foi iniciado)
-------------------------------------------------------------------------------

local function selfDir()
  local source = debug.getinfo(1, "S").source:sub(2)
  return source:match("(.*/)") or "./"
end

local replicateTaskFile = selfDir() .. "broker/replicate_task.lua"

-------------------------------------------------------------------------------
-- CONFIG
-------------------------------------------------------------------------------

local function loadConfig()
  if config == nil then
    if not os.exists(broker.configPath) then
      error("file " .. broker.configPath .. " not exists")
    end
    local raw  = template.execute(broker.configPath, {})
    local env  = mvm.env() or "development"
    local data = json.decode(raw)
    config = data[env]
  end
  return config
end

local function topicConfig(topic)
  local cfg    = loadConfig()
  local merged = {}
  for k, v in pairs(cfg.default or {}) do
    merged[k] = v
  end
  local override = cfg.topics and cfg.topics[topic]
  if override then
    for k, v in pairs(override) do
      merged[k] = v
    end
  end
  return merged
end

local function selfToken()
  local cfg = loadConfig()
  for _, peer in ipairs(cfg.peers or {}) do
    if peer.url == cfg.self then
      return peer.token
    end
  end
end

local function otherPeers()
  local cfg  = loadConfig()
  local list = {}
  for _, peer in ipairs(cfg.peers or {}) do
    if peer.url ~= cfg.self then
      table.insert(list, peer)
    end
  end
  return list
end

function broker.peers()
  return otherPeers()
end

function broker.isKnownPeerToken(token)
  local cfg = loadConfig()
  for _, peer in ipairs(cfg.peers or {}) do
    if peer.token == token then
      return true
    end
  end
  return false
end

-------------------------------------------------------------------------------
-- WRITE ATOMIC (tmp file + fsync + rename)
-------------------------------------------------------------------------------

local function writeAtomic(path, content)
  local tmpPath = path .. ".tmp"
  local file    = io.open(tmpPath, "wb")
  file:write(content)
  file:close()
  os.fsync(tmpPath)
  os.rename(tmpPath, path)
end

-------------------------------------------------------------------------------
-- NEXT SEQ (contador monotônico por tópico/agente, serializado por Lock)
-------------------------------------------------------------------------------

local function nextSeq(topic, agente)
  local lock    = Lock.new("arken.broker:seq:" .. topic .. ":" .. agente)
  local seqPath = broker.storagePath .. "/" .. topic .. "/" .. agente .. "/.seq"

  lock:enable()
  local current = os.exists(seqPath) and tonumber(os.read(seqPath)) or 0
  local seq     = current + 1
  writeAtomic(seqPath, tostring(seq))
  lock:disable()

  return seq
end

-- Réplicas (store) não alocam seq novo, mas precisam manter o marcador em
-- dia pra saber até onde já têm cópia (usado pelo catch-up).
local function bumpSeqMarker(topic, agente, seq)
  local lock    = Lock.new("arken.broker:seq:" .. topic .. ":" .. agente)
  local seqPath = broker.storagePath .. "/" .. topic .. "/" .. agente .. "/.seq"

  lock:enable()
  local current = os.exists(seqPath) and tonumber(os.read(seqPath)) or 0
  if seq > current then
    writeAtomic(seqPath, tostring(seq))
  end
  lock:disable()
end

function broker.lastSeq(topic, agente)
  local seqPath = broker.storagePath .. "/" .. topic .. "/" .. agente .. "/.seq"
  return os.exists(seqPath) and tonumber(os.read(seqPath)) or 0
end

-------------------------------------------------------------------------------
-- PERSIST (grava localmente; usado tanto por pub quanto por store)
-------------------------------------------------------------------------------

local function persist(params)
  local agenteDir = broker.storagePath .. "/" .. params.topic .. "/" .. params.agente
  local date      = os.date("%Y-%m-%d", math.floor(params.timestamp / 1000))
  local dirPath   = agenteDir .. "/" .. date
  os.mkpath(dirPath)

  local base = string.format("%09d-%d", params.seq, params.timestamp)
  if params.identificador then
    base = base .. "-" .. params.identificador
  end

  local fileName  = base .. "." .. params.ext
  local finalPath = dirPath .. "/" .. fileName

  writeAtomic(finalPath, params.body)

  local sha1 = os.sha1(finalPath)
  writeAtomic(finalPath .. ".sha1", sha1)

  if params.identificador then
    local byIdDir = agenteDir .. "/by-id"
    os.mkpath(byIdDir)
    local linkPath   = byIdDir .. "/" .. params.identificador .. "." .. params.ext
    local targetPath = "../" .. date .. "/" .. fileName
    os.link(targetPath, linkPath, true)
  end

  return {
    seq       = params.seq,
    timestamp = params.timestamp,
    path      = finalPath,
    sha1      = sha1
  }
end

-------------------------------------------------------------------------------
-- SCAN (varre os arquivos de mensagem de um agente; usado por catch-up e
-- pela limpeza por retenção)
-------------------------------------------------------------------------------

local function scanMessages(topic, agente)
  local agenteDir = broker.storagePath .. "/" .. topic .. "/" .. agente
  local result    = {}

  for path in os.find(agenteDir, ".*", true):each() do
    if (not os.isdir(path)) and (not path:contains("/by-id/")) then
      local name = path:match("([^/]+)$")
      if name
        and name ~= ".seq"
        and not name:endsWith(".sha1")
        and not name:endsWith(".tmp")
      then
        local seq, timestamp, identificador, ext = name:match("^(%d+)%-(%d+)%-?(.-)%.([^.]+)$")
        if seq then
          table.insert(result, {
            seq           = tonumber(seq),
            timestamp     = tonumber(timestamp),
            identificador = (#identificador > 0) and identificador or nil,
            ext           = ext,
            path          = path
          })
        end
      end
    end
  end

  table.sort(result, function(a, b) return a.seq < b.seq end)
  return result
end

function broker.listSince(topic, agente, afterSeq)
  afterSeq = afterSeq or 0
  local result = {}
  for _, item in ipairs(scanMessages(topic, agente)) do
    if item.seq > afterSeq then
      table.insert(result, item)
    end
  end
  return result
end

local function findBySeq(topic, agente, seq)
  for _, item in ipairs(scanMessages(topic, agente)) do
    if item.seq == seq then
      return item
    end
  end
end

-------------------------------------------------------------------------------
-- REPLICATE (fan-out síncrono pros peers configurados)
-------------------------------------------------------------------------------

local function replicateTo(peer, topic, agente, result, ext, identificador, body, timeoutSecs)
  local client = HttpClient.new(peer.url .. "/broker/replicate")

  client:appendHeader("Authorization: Bearer " .. (selfToken() or ""))
  client:appendHeader("X-Broker-Topic: "     .. topic)
  client:appendHeader("X-Broker-Agente: "    .. agente)
  client:appendHeader("X-Broker-Seq: "       .. tostring(result.seq))
  client:appendHeader("X-Broker-Timestamp: " .. tostring(result.timestamp))
  client:appendHeader("X-Broker-Ext: "       .. ext)
  if identificador then
    client:appendHeader("X-Broker-Identificador: " .. identificador)
  end

  client:setTimeout(timeoutSecs)
  client:setBody(body)
  client:performPost()

  return (not client:failure()) and client:status() == 200
end

local function replicateSync(topic, agente, result, ext, identificador, body, cfg)
  local timeoutSecs = math.max(1, math.ceil((cfg.timeout_ms or 5000) / 1000))
  local minReplicas = cfg.min_replicas or 0
  local confirmed    = 0

  for _, peer in ipairs(otherPeers()) do
    if replicateTo(peer, topic, agente, result, ext, identificador, body, timeoutSecs) then
      confirmed = confirmed + 1
      if confirmed >= minReplicas then
        break
      end
    end
  end

  if confirmed < minReplicas then
    error(string.format(
      "broker.pub: min_replicas not reached for topic '%s' (got %d, need %d)",
      topic, confirmed, minReplicas
    ))
  end
end

local function replicateAsync(topic, agente, result, ext, identificador, body, cfg)
  local retry      = cfg.retry or {}
  local attempts    = retry.enabled and (retry.attempts or 5) or 1
  local backoffMs   = retry.backoff_ms or 2000
  local token       = selfToken()

  for _, peer in ipairs(otherPeers()) do
    Task.start(replicateTaskFile, {
      url           = peer.url,
      token         = token,
      topic         = topic,
      agente        = agente,
      seq           = result.seq,
      timestamp     = result.timestamp,
      identificador = identificador,
      ext           = ext,
      data          = body,
      attempts      = attempts,
      backoff_ms    = backoffMs,
      timeout_ms    = cfg.timeout_ms
    }, true)
  end
end

-------------------------------------------------------------------------------
-- PUB
-- params: topic, agente, body (obrigatórios), identificador, ext (opcionais)
-------------------------------------------------------------------------------

function broker.pub(params)
  local topic         = assert(params.topic,  "topic is required")
  local agente        = assert(params.agente, "agente is required")
  local body           = params.body or ""
  local identificador = params.identificador
  local ext            = params.ext or "msg"

  os.mkpath(broker.storagePath .. "/" .. topic .. "/" .. agente)

  local seq       = nextSeq(topic, agente)
  local timestamp = math.floor(os.microtime() * 1000)

  local result = persist{
    topic         = topic,
    agente        = agente,
    seq           = seq,
    timestamp     = timestamp,
    identificador = identificador,
    ext           = ext,
    body          = body
  }

  local cfg = topicConfig(topic)
  if #otherPeers() > 0 then
    if cfg.delivery == "sync" then
      replicateSync(topic, agente, result, ext, identificador, body, cfg)
    else
      replicateAsync(topic, agente, result, ext, identificador, body, cfg)
    end
  end

  return result
end

-------------------------------------------------------------------------------
-- STORE
-- grava réplica vinda de outro nó; seq/timestamp já foram decididos na origem
-------------------------------------------------------------------------------

function broker.store(params)
  local topic     = assert(params.topic,  "topic is required")
  local agente    = assert(params.agente, "agente is required")
  local seq       = assert(tonumber(params.seq),       "seq is required")
  local timestamp = assert(tonumber(params.timestamp), "timestamp is required")

  local result = persist{
    topic         = topic,
    agente        = agente,
    seq           = seq,
    timestamp     = timestamp,
    identificador = params.identificador,
    ext           = params.ext or "msg",
    body          = params.body or ""
  }

  bumpSeqMarker(topic, agente, seq)

  return result
end

-------------------------------------------------------------------------------
-- READ (descritor + corpo de uma mensagem específica; usado pelo endpoint
-- de fetch do catch-up)
-------------------------------------------------------------------------------

function broker.read(topic, agente, seq)
  local item = findBySeq(topic, agente, tonumber(seq))
  if not item then
    return nil
  end
  item.body = os.read(item.path)
  return item
end

-------------------------------------------------------------------------------
-- CATCH-UP (cliente: puxa de um peer tudo que falta desde o último seq local)
-------------------------------------------------------------------------------

function broker.catchUp(peer, topic, agente)
  local cfg          = topicConfig(topic)
  local timeoutSecs   = math.max(1, math.ceil((cfg.timeout_ms or 5000) / 1000))
  local afterSeq      = broker.lastSeq(topic, agente)
  local token         = selfToken()

  local list = HttpClient.new(string.format(
    "%s/broker/catchup?topic=%s&agente=%s&after=%d",
    peer.url, topic, agente, afterSeq
  ))
  list:appendHeader("Authorization: Bearer " .. (token or ""))
  list:setTimeout(timeoutSecs)
  local raw = list:performGet()

  if list:failure() or list:status() ~= 200 then
    error("broker.catchUp: failed to list from peer " .. peer.url)
  end

  local items   = json.decode(raw)
  local applied = 0

  for _, item in ipairs(items) do
    local fetch = HttpClient.new(string.format(
      "%s/broker/fetch?topic=%s&agente=%s&seq=%d",
      peer.url, topic, agente, item.seq
    ))
    fetch:appendHeader("Authorization: Bearer " .. (token or ""))
    fetch:setTimeout(timeoutSecs)
    local body = fetch:performGet()

    if fetch:failure() or fetch:status() ~= 200 then
      error(string.format("broker.catchUp: failed to fetch seq %d from peer %s", item.seq, peer.url))
    end

    broker.store{
      topic         = topic,
      agente        = agente,
      seq           = item.seq,
      timestamp     = item.timestamp,
      identificador = item.identificador,
      ext           = item.ext,
      body          = body
    }
    applied = applied + 1
  end

  return applied
end

-------------------------------------------------------------------------------
-- LIST (enumera pares tópico/agente conhecidos localmente)
-------------------------------------------------------------------------------

function broker.list()
  local result = {}
  for dir in os.glob(broker.storagePath .. "/*/*"):each() do
    if os.isdir(dir) then
      table.insert(result, {
        topic  = dir:match("([^/]+)/[^/]+$"),
        agente = dir:match("([^/]+)$")
      })
    end
  end
  return result
end

-------------------------------------------------------------------------------
-- CLEANUP (apaga pastas de data mais velhas que retention_days do tópico, e
-- os symlinks em by-id que ficaram apontando pra nada)
-------------------------------------------------------------------------------

function broker.cleanup(topic, agente)
  local cfg           = topicConfig(topic)
  local retentionDays = cfg.retention_days or 3
  local cutoff         = os.date("%Y-%m-%d", os.microtime() - (retentionDays * 86400))
  local agenteDir      = broker.storagePath .. "/" .. topic .. "/" .. agente
  local removed        = {}

  for entry in os.glob(agenteDir .. "/*"):each() do
    local name = entry:match("([^/]+)$")
    if name and name:match("^%d%d%d%d%-%d%d%-%d%d$") and name < cutoff then
      os.rmpath(entry)
      table.insert(removed, name)
    end
  end

  local byIdDir = agenteDir .. "/by-id"
  if os.isdir(byIdDir) then
    for entry in os.glob(byIdDir .. "/*"):each() do
      if os.islink(entry) and not os.exists(entry) then
        os.remove(entry)
      end
    end
  end

  return removed
end

return broker
