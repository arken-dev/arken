-- Fixture usada por tests/lib/arken/concurrent/mailbox: envia params.count
-- mensagens (params.prefix .. índice) para a mailbox, protegendo cada send()
-- com pcall (send() lança erro se a mailbox estiver fechada, cenário
-- esperado quando testado em conjunto com close() concorrente). Reporta
-- quantas mensagens foram aceitas e quantas falharam.
local Mailbox = require("arken.concurrent.mailbox")

return function(task, params)
  local mailbox = Mailbox.open(params.mailbox)
  local report  = Mailbox.open(params.report)

  local ok, failed = 0, 0

  for i = 1, params.count do
    local status = pcall(function()
      mailbox:send(params.prefix .. tostring(i))
    end)
    if status then
      ok = ok + 1
    else
      failed = failed + 1
    end
  end

  report:send({ ok = ok, failed = failed })
end
