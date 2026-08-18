local Mailbox = require("arken.concurrent.mailbox")
local task    = require("arken.concurrent.task")

local test = {}

local fixture = os.pwd() .. '/tests/fixtures/concurrent/mailbox/singleReceive.lua'

-- Teste 3 — receive bloqueante
test['should block a consumer with no messages until a producer sends one'] = function()
  local mailbox = Mailbox.open('tests-mailbox-receive-blocking')
  local report  = Mailbox.open('tests-mailbox-receive-blocking-report')

  local consumer = task.start(fixture, { mailbox = 'tests-mailbox-receive-blocking', report = 'tests-mailbox-receive-blocking-report' }, false)

  -- dá tempo do consumidor entrar em receive() e dormir na condition_variable
  os.sleep(0.3)
  assert( report:fetch() == nil, 'consumer must still be blocked, nothing sent yet' )

  mailbox:send('woke-up')
  consumer:wait()

  local result = report:receive()
  assert( result.woke == true )
  assert( result.message == 'woke-up' )

  mailbox:close()
  report:close()
end

-- Teste 8 — close acordando receive
test['should wake a consumer blocked on receive() when the mailbox is closed'] = function()
  local mailbox = Mailbox.open('tests-mailbox-receive-close-wakes')
  local report  = Mailbox.open('tests-mailbox-receive-close-wakes-report')

  local consumer = task.start(fixture, { mailbox = 'tests-mailbox-receive-close-wakes', report = 'tests-mailbox-receive-close-wakes-report' }, false)

  os.sleep(0.3)
  mailbox:close()
  consumer:wait()

  local result = report:receive()
  assert( result.woke == true )
  assert( result.message == nil )

  report:close()
end

-- Teste 9 — drain após close
test['should drain queued messages after close before returning nil'] = function()
  local mailbox = Mailbox.open('tests-mailbox-receive-drain')

  mailbox:send('A')
  mailbox:send('B')
  mailbox:send('C')
  mailbox:close()

  assert( mailbox:receive() == 'A' )
  assert( mailbox:receive() == 'B' )
  assert( mailbox:receive() == 'C' )
  assert( mailbox:receive() == nil )
end

return test
