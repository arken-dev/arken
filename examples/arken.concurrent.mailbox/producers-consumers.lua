-- arken.concurrent.mailbox example: múltiplos produtores e consumidores
--
-- Um padrão fan-out/fan-in comum: N produtores enfileirando trabalho e M
-- consumidores (workers) processando em paralelo, cada job indo pra
-- exatamente um consumidor. A Mailbox não sabe quantos produtores ou
-- consumidores existem — apenas garante FIFO e entrega única por mensagem.

local Mailbox = require('arken.concurrent.mailbox')
local task    = require('arken.concurrent.task')

local node = os.pwd() .. '/examples/arken.concurrent.mailbox/producers-consumers-node.lua'

local mailbox = Mailbox.open('work-queue')

-- os consumidores sobem primeiro e ficam bloqueados em receive() — assim
-- os jobs chegam com os dois já disputando a fila, em vez de um deles
-- drenar tudo antes do outro sequer começar
local consumers = {}
for i = 1, 2 do
  consumers[i] = task.start(node, { role = 'consumer', id = i }, false)
end

-- 3 produtores, cada um enfileirando 4 jobs
local producers = {}
for i = 1, 3 do
  producers[i] = task.start(node, { role = 'producer', id = i, count = 4 }, false)
end

for i = 1, #producers do
  producers[i]:wait()
end

-- não há mais jobs chegando: fecha a mailbox para que os consumidores,
-- depois de drenar o que sobrou, terminem sozinhos ao invés de ficar
-- bloqueados pra sempre em receive()
mailbox:close()

for i = 1, #consumers do
  consumers[i]:wait()
end
