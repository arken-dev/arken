-- arken.concurrent.mailbox example: worker bloqueado em receive() + shutdown
--
-- worker-node.lua roda numa thread separada (arken.concurrent.task) e fica
-- bloqueado em mailbox:receive() sem consumir CPU enquanto não há trabalho.
-- Esta thread principal (o "produtor") envia algumas mensagens, dá um
-- tempo pro worker processá-las e então chama close(): o worker acorda,
-- drena o que sobrou na fila e termina sozinho — sem callbacks, sem
-- polling, sem a Mailbox saber (ou precisar saber) o que "order.updated"
-- significa.

local Mailbox = require('arken.concurrent.mailbox')
local task    = require('arken.concurrent.task')

local mailbox = Mailbox.open('orders')

local worker = task.start(os.pwd() .. '/examples/arken.concurrent.mailbox/worker-node.lua', {}, false)

mailbox:send({ order_id = 1 })
mailbox:send({ order_id = 2 })
mailbox:send({ order_id = 3 })

-- dá tempo do worker processar antes de fechar
os.sleep(0.5)

-- não há mais pedidos: fecha a mailbox. Isso acorda o worker se ele
-- estiver bloqueado em receive(), e permite drenar mensagens que ainda
-- estejam na fila antes dele terminar.
mailbox:close()

-- espera a thread do worker terminar antes do processo sair
worker:wait()
