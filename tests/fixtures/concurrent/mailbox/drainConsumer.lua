-- Fixture usada por tests/lib/arken/concurrent/mailbox: consome via receive()
-- até a mailbox ser fechada e drenada (receive() == nil), acumulando tudo
-- que recebeu, e reporta a lista completa numa mailbox de resultado.
local Mailbox = require("arken.concurrent.mailbox")

return function(task, params)
  local mailbox = Mailbox.open(params.mailbox)
  local report  = Mailbox.open(params.report)

  local received = {}
  while true do
    local message = mailbox:receive()
    if message == nil then
      break
    end
    table.insert(received, message)
  end

  report:send(received)
end
