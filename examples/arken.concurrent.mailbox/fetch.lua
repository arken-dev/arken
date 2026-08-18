-- arken.concurrent.mailbox example: fetch() (não bloqueante) vs receive() (bloqueante)
--
-- fetch() tenta obter a próxima mensagem agora; se a fila estiver vazia,
-- devolve nil imediatamente, sem esperar. É a ferramenta certa quando o
-- consumidor não pode ficar parado esperando — por exemplo, checando uma
-- fila de tempos em tempos dentro de um loop que também faz outras coisas.

local Mailbox = require('arken.concurrent.mailbox')

local mailbox = Mailbox.open('notifications')

-- fila vazia: fetch() retorna nil na hora, não bloqueia
print('fetch on empty queue => ' .. tostring(mailbox:fetch()))

mailbox:send('first notification')
mailbox:send('second notification')

-- com mensagens na fila, fetch() se comporta como receive(): remove e
-- retorna a próxima, respeitando FIFO
print('fetch => ' .. mailbox:fetch())
print('fetch => ' .. mailbox:fetch())

-- drenou tudo: volta a retornar nil sem bloquear
print('fetch on drained queue => ' .. tostring(mailbox:fetch()))

mailbox:close()
