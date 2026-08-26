local Mailbox = require("arken.concurrent.mailbox")

local test = {}

-- Teste 4 — fetch vazio
test['should return nil immediately when the queue is empty'] = function()
  local mailbox = Mailbox.open('tests-mailbox-fetch-empty')

  -- se fetch() bloqueasse por engano, este teste travaria (não há timeout);
  -- retornar aqui já é a prova de que não bloqueou.
  assert( mailbox:fetch() == nil )

  mailbox:close()
end

-- Teste 5 — fetch com mensagem
test['should remove and return the next message without blocking'] = function()
  local mailbox = Mailbox.open('tests-mailbox-fetch-message')

  mailbox:send('A')

  assert( mailbox:fetch() == 'A' )
  assert( mailbox:fetch() == nil )

  mailbox:close()
end

test['should preserve FIFO order across fetch() calls'] = function()
  local mailbox = Mailbox.open('tests-mailbox-fetch-fifo')

  mailbox:send('A')
  mailbox:send('B')

  assert( mailbox:fetch() == 'A' )
  assert( mailbox:fetch() == 'B' )
  assert( mailbox:fetch() == nil )

  mailbox:close()
end

return test
