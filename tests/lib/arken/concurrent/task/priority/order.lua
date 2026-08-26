-- arken.concurrent.task.priority: one global queue, higher priority first
--
-- No key/name parameter -- one shared queue backed by a
-- std::priority_queue. A HIGHER priority value is dequeued first; ties
-- break by submission order. With max(1), submitting low priority first
-- and high priority right after still finishes the high-priority task
-- first.

local priority = require('arken.concurrent.task.priority')
local mvm      = require('arken.mvm')
local worker   = 'support/concurrent_task/recorder_worker.lua'
local test     = {}

test.higher_priority_finishes_first_even_if_submitted_later = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:priority:order:' .. tostring(os.microtime())

  priority.max(1)

  -- submitted first, but LOW priority
  priority.start(worker, { bucket = bucket, tag = 'low',    sleep = 0.05 }, 1)
  -- submitted right after, HIGH priority
  priority.start(worker, { bucket = bucket, tag = 'high',   sleep = 0.05 }, 9)
  -- submitted last, MEDIUM priority
  priority.start(worker, { bucket = bucket, tag = 'medium', sleep = 0.05 }, 5)

  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 3, #intervals )
  timeline.assertNoOverlap(intervals)

  assert( intervals[1].tag == 'high', intervals[1].tag )
  assert( intervals[2].tag == 'medium', intervals[2].tag )
  assert( intervals[3].tag == 'low', intervals[3].tag )
end

return test
