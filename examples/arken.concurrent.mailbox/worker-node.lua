-- Nó do exemplo worker.lua: roda numa VM/thread separada (via
-- arken.concurrent.task), consumindo a mailbox "orders" até ela ser
-- fechada. mailbox:receive() bloqueia essa thread sem consumir CPU
-- enquanto não houver mensagem — é o mecanismo de condition_variable da
-- Mailbox, não polling.

local Mailbox = require('arken.concurrent.mailbox')

return function(task, params)
  local mailbox = Mailbox.open('orders')

  while true do
    local message = mailbox:receive()

    if message == nil then
      -- mailbox fechada e drenada: hora de terminar
      break
    end

    print('[worker] processing order ' .. message.order_id)
  end

  print('[worker] mailbox closed, shutting down')
end
