-- Fixture usada por tests/lib/arken/concurrent/mailbox via arken.concurrent.task:
-- bloqueia em receive() uma única vez e reporta o resultado numa mailbox à
-- parte, para que o teste consiga observar (do thread principal) o momento
-- em que o consumidor acorda.
local Mailbox = require("arken.concurrent.mailbox")

return function(task, params)
  local mailbox = Mailbox.open(params.mailbox)
  local report  = Mailbox.open(params.report)

  local message = mailbox:receive()
  report:send({ woke = true, message = message })
end
