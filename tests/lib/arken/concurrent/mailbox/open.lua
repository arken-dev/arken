local Mailbox = require("arken.concurrent.mailbox")

local test = {}

test['should share the same underlying state across separate open() calls with the same name'] = function()
  local a = Mailbox.open('tests-mailbox-open-shared')
  local b = Mailbox.open('tests-mailbox-open-shared')

  a:send('from-a')

  assert( b:fetch() == 'from-a' )

  a:close()
end

test['should isolate state between mailboxes opened with different names'] = function()
  local a = Mailbox.open('tests-mailbox-open-isolated-a')
  local b = Mailbox.open('tests-mailbox-open-isolated-b')

  a:send('only-in-a')

  assert( b:fetch() == nil )
  assert( a:fetch() == 'only-in-a' )

  a:close()
  b:close()
end

return test
