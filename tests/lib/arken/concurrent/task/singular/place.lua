-- arken.concurrent.task.singular: place() skip condition
--
-- place(file, params, key) queues only if runners().count(key) == 0.
-- That entry exists from the moment key is first used until its queue
-- fully drains AND the last running task for it finishes -- stricter
-- than emplace(), which only looks at the queue. So a second place()
-- call is skipped both while the first task is queued AND while it is
-- already running.

local singular = require('arken.concurrent.task.singular')
local mvm      = require('arken.mvm')
local worker   = 'support/concurrent_task/recorder_worker.lua'
local test     = {}

test.second_call_is_skipped_while_a_runner_is_tracked = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:singular:place:' .. tostring(os.microtime())
  local key      = 'account:place-1'

  singular.place(worker, { bucket = bucket, tag = 'first', sleep = 0.2 }, key)
  -- called again while "first" is still running, unlike emplace() this
  -- is skipped even though the QUEUE is already empty at this point
  singular.place(worker, { bucket = bucket, tag = 'second', sleep = 0.2 }, key)

  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 1, #intervals )
  assert( intervals[1].tag == 'first', intervals[1].tag )
end

test.queues_again_once_previous_task_fully_finished = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:singular:place-requeue:' .. tostring(os.microtime())
  local key      = 'account:place-2'

  singular.place(worker, { bucket = bucket, tag = 'first', sleep = 0.05 }, key)
  mvm.wait()

  singular.place(worker, { bucket = bucket, tag = 'second', sleep = 0.05 }, key)
  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 2, #intervals )
  assert( intervals[1].tag == 'first', intervals[1].tag )
  assert( intervals[2].tag == 'second', intervals[2].tag )
end

return test
