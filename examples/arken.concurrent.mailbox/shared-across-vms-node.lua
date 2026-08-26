-- Nó do exemplo shared-across-vms.lua: representa a "VM B". Abre a mesma
-- mailbox nomeada que a VM A abriu e recebe a mensagem que ela enviou — a
-- Mailbox não sabe (nem precisa saber) que existem duas VMs envolvidas.

local Mailbox = require('arken.concurrent.mailbox')

return function(task, params)
  local mailbox = Mailbox.open('marketplace')

  local message = mailbox:receive()
  print('[VM B] received: action=' .. message.action .. ' marketplace_id=' .. message.marketplace_id)
end
