-- Corpo da task de replicação assíncrona (roda numa VM Lua separada, em
-- background). Recebe topic/agente/seq/timestamp/ext/identificador e o corpo
-- binário via params (json encode/decode, testado com dados aleatórios e
-- bytes nulos — sobrevive intacto), e tenta entregar pro peer, com retry e
-- backoff conforme configurado no tópico.

return function(task, params)
  local HttpClient = require "arken.net.HttpClient"

  local attempts    = params.attempts   or 1
  local backoffSecs = (params.backoff_ms or 2000) / 1000
  local timeoutSecs = math.max(1, math.ceil((params.timeout_ms or 5000) / 1000))

  local ok = false

  for attempt = 1, attempts do
    local client = HttpClient.new(params.url .. "/broker/replicate")

    client:appendHeader("Authorization: Bearer " .. (params.token or ""))
    client:appendHeader("X-Broker-Topic: "     .. params.topic)
    client:appendHeader("X-Broker-Agente: "    .. params.agente)
    client:appendHeader("X-Broker-Seq: "       .. tostring(params.seq))
    client:appendHeader("X-Broker-Timestamp: " .. tostring(params.timestamp))
    client:appendHeader("X-Broker-Ext: "       .. params.ext)
    if params.identificador then
      client:appendHeader("X-Broker-Identificador: " .. params.identificador)
    end

    client:setTimeout(timeoutSecs)
    client:setBody(params.data or "")
    client:performPost()

    if (not client:failure()) and client:status() == 200 then
      ok = true
      break
    end

    if attempt < attempts then
      os.sleep(backoffSecs)
    end
  end

  task:shared():setString("ok", tostring(ok))
end
