local broker = require "arken.broker"
local json   = require "arken.json"

local BrokerController = Class.new("BrokerController", "Controller")

local function jsonResponse(code, data)
  return code, {'Content-Type: application/json; charset=utf-8'}, json.encode(data)
end

function BrokerController:indexAction()
  local env            = self:env()
  local topic          = env:field("X-Broker-Topic")
  local agente         = env:field("X-Broker-Agente")
  local identificador  = env:field("X-Broker-Identificador")
  local ext            = env:field("X-Broker-Ext")

  if #topic == 0 then
    return jsonResponse(400, { error = "X-Broker-Topic header is required" })
  end

  if #agente == 0 then
    return jsonResponse(400, { error = "X-Broker-Agente header is required" })
  end

  local ok, result = pcall(broker.pub, {
    topic         = topic,
    agente        = agente,
    identificador = (#identificador > 0) and identificador or nil,
    ext           = (#ext > 0) and ext or nil,
    body          = env:headerDone()
  })

  if not ok then
    return jsonResponse(500, { error = tostring(result) })
  end

  return jsonResponse(200, result)
end

-------------------------------------------------------------------------------
-- REPLICATE (chamado por outro nó do broker, autenticado por token)
-------------------------------------------------------------------------------

function BrokerController:replicateAction()
  local env   = self:env()
  local token = env:field("Authorization"):replace("Bearer ", "")

  if #token == 0 or not broker.isKnownPeerToken(token) then
    return jsonResponse(401, { error = "invalid or missing peer token" })
  end

  local topic         = env:field("X-Broker-Topic")
  local agente         = env:field("X-Broker-Agente")
  local seq            = env:field("X-Broker-Seq")
  local timestamp       = env:field("X-Broker-Timestamp")
  local identificador  = env:field("X-Broker-Identificador")
  local ext             = env:field("X-Broker-Ext")

  if #topic == 0 or #agente == 0 or #seq == 0 or #timestamp == 0 then
    return jsonResponse(400, { error = "X-Broker-Topic, X-Broker-Agente, X-Broker-Seq and X-Broker-Timestamp headers are required" })
  end

  local ok, result = pcall(broker.store, {
    topic         = topic,
    agente        = agente,
    seq           = seq,
    timestamp     = timestamp,
    identificador = (#identificador > 0) and identificador or nil,
    ext           = (#ext > 0) and ext or nil,
    body          = env:headerDone()
  })

  if not ok then
    return jsonResponse(500, { error = tostring(result) })
  end

  return jsonResponse(200, result)
end

-------------------------------------------------------------------------------
-- CATCHUP (lista o que existe além de um seq; chamado por outro nó)
-- GET /broker/catchup?topic=&agente=&after=
-------------------------------------------------------------------------------

function BrokerController:catchupAction()
  local env   = self:env()
  local token = env:field("Authorization"):replace("Bearer ", "")

  if #token == 0 or not broker.isKnownPeerToken(token) then
    return jsonResponse(401, { error = "invalid or missing peer token" })
  end

  local params = self:params()
  local topic  = params.topic or ""
  local agente = params.agente or ""
  local after  = tonumber(params.after) or 0

  if #topic == 0 or #agente == 0 then
    return jsonResponse(400, { error = "topic and agente query params are required" })
  end

  local list = {}
  for _, item in ipairs(broker.listSince(topic, agente, after)) do
    table.insert(list, {
      seq           = item.seq,
      timestamp     = item.timestamp,
      identificador = item.identificador,
      ext           = item.ext
    })
  end

  return jsonResponse(200, list)
end

-------------------------------------------------------------------------------
-- FETCH (devolve o conteúdo bruto de uma mensagem específica)
-- GET /broker/fetch?topic=&agente=&seq=
-------------------------------------------------------------------------------

function BrokerController:fetchAction()
  local env   = self:env()
  local token = env:field("Authorization"):replace("Bearer ", "")

  if #token == 0 or not broker.isKnownPeerToken(token) then
    return jsonResponse(401, { error = "invalid or missing peer token" })
  end

  local params = self:params()
  local topic  = params.topic or ""
  local agente = params.agente or ""
  local seq    = tonumber(params.seq)

  if #topic == 0 or #agente == 0 or not seq then
    return jsonResponse(400, { error = "topic, agente and seq query params are required" })
  end

  local item = broker.read(topic, agente, seq)
  if not item then
    return jsonResponse(404, { error = "message not found" })
  end

  local headers = {
    'Content-Type: application/octet-stream',
    "X-Broker-Timestamp: " .. tostring(item.timestamp),
    "X-Broker-Ext: "       .. item.ext
  }
  if item.identificador then
    table.insert(headers, "X-Broker-Identificador: " .. item.identificador)
  end

  return 200, headers, item.body
end

return BrokerController
