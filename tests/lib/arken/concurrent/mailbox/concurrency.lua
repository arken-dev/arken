local Mailbox = require("arken.concurrent.mailbox")
local task    = require("arken.concurrent.task")

local test = {}

local producerFixture = os.pwd() .. '/tests/fixtures/concurrent/mailbox/producer.lua'
local drainFixture     = os.pwd() .. '/tests/fixtures/concurrent/mailbox/drainConsumer.lua'

-- Teste 7 — múltiplos produtores
test['should keep the queue consistent under multiple concurrent producers'] = function()
  local name       = 'tests-mailbox-concurrency-producers'
  local reportName = 'tests-mailbox-concurrency-producers-report'
  local producers  = 5
  local perProducer = 20

  local mailbox = Mailbox.open(name)
  local report  = Mailbox.open(reportName)

  local tasks = {}
  for i = 1, producers do
    tasks[i] = task.start(producerFixture, {
      mailbox = name,
      report  = reportName,
      prefix  = 'p' .. tostring(i) .. '-',
      count   = perProducer,
    }, false)
  end

  local totalOk = 0
  for i = 1, producers do
    tasks[i]:wait()
    local result = report:receive()
    assert( result.failed == 0, 'no send should fail while the mailbox stays open' )
    totalOk = totalOk + result.ok
  end

  assert( totalOk == producers * perProducer )

  local seen  = {}
  local count = 0
  while true do
    local message = mailbox:fetch()
    if message == nil then break end
    assert( seen[message] == nil, 'message delivered more than once: ' .. tostring(message) )
    seen[message] = true
    count = count + 1
  end

  assert( count == producers * perProducer, 'expected ' .. (producers * perProducer) .. ' messages, got ' .. count )

  mailbox:close()
  report:close()
end

-- Teste 6 — múltiplos consumidores
test['should deliver exactly one copy of each message across multiple consumers'] = function()
  local name       = 'tests-mailbox-concurrency-consumers'
  local reportName = 'tests-mailbox-concurrency-consumers-report'
  local consumers  = 3
  local total      = 30

  local mailbox = Mailbox.open(name)
  local report  = Mailbox.open(reportName)

  for i = 1, total do
    mailbox:send('m' .. tostring(i))
  end
  -- fecha antes de soltar os consumidores: cada um drena o que sobrar e
  -- termina assim que a fila esvaziar, sem precisar de sincronização extra.
  mailbox:close()

  local tasks = {}
  for i = 1, consumers do
    tasks[i] = task.start(drainFixture, { mailbox = name, report = reportName }, false)
  end

  local seen  = {}
  local count = 0
  for i = 1, consumers do
    tasks[i]:wait()
    local received = report:receive()
    for _, message in ipairs(received) do
      assert( seen[message] == nil, 'message delivered more than once: ' .. tostring(message) )
      seen[message] = true
      count = count + 1
    end
  end

  assert( count == total, 'expected ' .. total .. ' messages across all consumers, got ' .. count )

  report:close()
end

-- Teste 12 — concorrência close/send
test['should stay consistent when send() and close() race from multiple threads'] = function()
  local name       = 'tests-mailbox-concurrency-close-send'
  local reportName = 'tests-mailbox-concurrency-close-send-report'
  local producers   = 5
  local perProducer = 50

  local mailbox = Mailbox.open(name)
  local report  = Mailbox.open(reportName)

  local tasks = {}
  for i = 1, producers do
    tasks[i] = task.start(producerFixture, {
      mailbox = name,
      report  = reportName,
      prefix  = 'r' .. tostring(i) .. '-',
      count   = perProducer,
    }, false)
  end

  -- corrida deliberada: fecha enquanto os produtores ainda podem estar
  -- enviando.
  mailbox:close()

  local acceptedTotal, attemptedTotal = 0, 0
  for i = 1, producers do
    tasks[i]:wait()
    local result = report:receive()
    acceptedTotal   = acceptedTotal + result.ok
    attemptedTotal  = attemptedTotal + result.ok + result.failed
  end

  assert( attemptedTotal == producers * perProducer, 'every send attempt must be accounted for as ok or failed' )
  assert( mailbox:isClosed() == true )

  local queued = 0
  while true do
    local message = mailbox:fetch()
    if message == nil then break end
    queued = queued + 1
  end

  -- nenhuma mensagem aceita pode ter sido perdida, e nenhuma mensagem a
  -- mais (fantasma) pode ter aparecido na fila
  assert( queued == acceptedTotal, 'accepted=' .. acceptedTotal .. ' queued=' .. queued )

  report:close()
end

return test
