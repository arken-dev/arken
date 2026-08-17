-- arken.concurrent.task.singular: emplace() skip condition
--
-- emplace(file, params, key) queues only if map()[key] (the QUEUE) is
-- empty. Called twice back-to-back with no yield in between, the second
-- call always sees a non-empty queue (the first item hasn't been
-- dequeued yet), so it is skipped -- deterministically, not a race.

local singular = require('arken.concurrent.task.singular')
local mvm      = require('arken.mvm')
local worker   = 'support/concurrent_task/recorder_worker.lua'
local test     = {}

test.second_call_is_skipped_while_queue_is_non_empty = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:singular:emplace:' .. tostring(os.microtime())
  local key      = 'account:emplace-1'

  singular.emplace(worker, { bucket = bucket, tag = 'first',  sleep = 0.2 }, key)
  singular.emplace(worker, { bucket = bucket, tag = 'second', sleep = 0.2 }, key)

  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 1, #intervals )
  assert( intervals[1].tag == 'first', intervals[1].tag )
end

test.queues_again_once_previous_call_left_the_queue = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:singular:emplace-requeue:' .. tostring(os.microtime())
  local key      = 'account:emplace-2'

  singular.emplace(worker, { bucket = bucket, tag = 'first', sleep = 0.05 }, key)
  mvm.wait()

  -- first call has fully finished (queue and runner both cleared), so
  -- this one queues normally instead of being skipped
  singular.emplace(worker, { bucket = bucket, tag = 'second', sleep = 0.05 }, key)
  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 2, #intervals )
  assert( intervals[1].tag == 'first', intervals[1].tag )
  assert( intervals[2].tag == 'second', intervals[2].tag )
end

return test
