-- arken.concurrent.mailbox example: comunicação entre VMs
--
-- Mailbox.open(nome) sempre aponta pro mesmo estado compartilhado, não
-- importa em qual VM/thread ele é chamado. Aqui, esta thread principal
-- ("VM A") inicia shared-across-vms-node.lua numa VM separada ("VM B", via
-- arken.concurrent.task) e manda uma mensagem pra ela através da mailbox
-- "marketplace" — a mesma primitiva que os outros exemplos usam dentro de
-- uma única thread também funciona, sem mudança nenhuma, entre threads.

local Mailbox = require('arken.concurrent.mailbox')
local task    = require('arken.concurrent.task')

local mailbox = Mailbox.open('marketplace')

local vmB = task.start(os.pwd() .. '/examples/arken.concurrent.mailbox/shared-across-vms-node.lua', {}, false)

-- dá tempo da VM B abrir a mailbox e bloquear em receive()
os.sleep(0.3)

print('[VM A] sending message')
mailbox:send({
  action        = 'reconcile',
  marketplace_id = 1234,
})

vmB:wait()
