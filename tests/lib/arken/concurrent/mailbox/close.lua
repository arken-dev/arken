local Mailbox = require("arken.concurrent.mailbox")

local test = {}

test['isClosed() should reflect open vs closed state'] = function()
  local mailbox = Mailbox.open('tests-mailbox-close-isClosed')

  assert( mailbox:isClosed() == false )
  mailbox:close()
  assert( mailbox:isClosed() == true )
end

-- Teste 11 — close idempotente
test['close() should be idempotent'] = function()
  local mailbox = Mailbox.open('tests-mailbox-close-idempotent')

  mailbox:close()
  mailbox:close()
  mailbox:close()

  assert( mailbox:isClosed() == true )
end

test['close() should not discard messages already queued'] = function()
  local mailbox = Mailbox.open('tests-mailbox-close-preserves-queue')

  mailbox:send('kept')
  mailbox:close()

  assert( mailbox:isClosed() == true )
  assert( mailbox:fetch() == 'kept' )
end

return test
