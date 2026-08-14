local Mailbox = require("arken.concurrent.mailbox")

local test = {}

-- Teste 1 — send/receive
test['should deliver a sent message to receive()'] = function()
  local mailbox = Mailbox.open('tests-mailbox-send-basic')

  mailbox:send({ action = 'order.updated', order_id = 1234 })

  local message = mailbox:receive()
  assert( message.action == 'order.updated' )
  assert( message.order_id == 1234 )

  mailbox:close()
end

test['should transport strings, numbers and booleans as-is'] = function()
  local mailbox = Mailbox.open('tests-mailbox-send-scalars')

  mailbox:send('hello')
  mailbox:send(42)
  mailbox:send(true)

  assert( mailbox:receive() == 'hello' )
  assert( mailbox:receive() == 42 )
  assert( mailbox:receive() == true )

  mailbox:close()
end

-- Teste 2 — FIFO
test['should keep FIFO order across multiple sends from a single producer'] = function()
  local mailbox = Mailbox.open('tests-mailbox-send-fifo')

  mailbox:send('message1')
  mailbox:send('message2')
  mailbox:send('message3')

  assert( mailbox:receive() == 'message1' )
  assert( mailbox:receive() == 'message2' )
  assert( mailbox:receive() == 'message3' )

  mailbox:close()
end

-- Teste 10 — send após close
test['should error deterministically when sending after close, without accepting the message'] = function()
  local mailbox = Mailbox.open('tests-mailbox-send-after-close')

  mailbox:send('kept')
  mailbox:close()

  local ok, err = pcall(function() mailbox:send('rejected') end)

  assert( ok == false )
  assert( tostring(err):contains('closed') )

  -- a mensagem enviada antes do close() permanece; a rejeitada nunca entrou
  assert( mailbox:receive() == 'kept' )
  assert( mailbox:receive() == nil )
end

return test
