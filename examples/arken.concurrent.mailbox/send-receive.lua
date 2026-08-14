-- arken.concurrent.mailbox example: send / receive básico
--
-- Mailbox.open(nome) acessa (criando na primeira vez) uma fila FIFO nomeada
-- de mensagens. send() nunca bloqueia o produtor; receive() bloqueia o
-- consumidor até existir mensagem — aqui, como tudo roda no mesmo thread
-- em sequência, cada receive() sempre encontra a fila não vazia e retorna
-- na hora.
--
-- A Mailbox é agnóstica ao conteúdo: qualquer valor Lua (string, número,
-- booleano, tabela) pode ser enviado como mensagem.

local Mailbox = require('arken.concurrent.mailbox')

local mailbox = Mailbox.open('orders')

mailbox:send({
  action   = 'order.updated',
  order_id = 1234,
})
mailbox:send('a simple string message')
mailbox:send(42)

-- FIFO: a ordem de recebimento é a ordem de envio
local order = mailbox:receive()
print('action    = ' .. order.action)
print('order_id  = ' .. order.order_id)

print('message   = ' .. mailbox:receive())
print('number    = ' .. mailbox:receive())

mailbox:close()
