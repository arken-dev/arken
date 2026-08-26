-- arken.concurrent.task.scheduled: fairness across keys
--
-- Structurally like singular (map()/runners()/vector()/position() keyed
-- by name), but runners() here is a monotonically increasing
-- "times-served" COUNTER, never decremented when a task finishes. Each
-- dequeue() scans the keys (starting at position(), wrapping around) and
-- picks whichever pending key has been served the FEWEST times so far --
-- so a key with a deep backlog cannot starve a key with little work.
--
-- This is NOT exclusivity: scheduled only decides which key's queue to
-- pull from next, it says nothing about whether two tasks for the same
-- key can run at once (see singular for that guarantee). With max(1)
-- there is only one worker, so this test isolates pure ordering.
--
-- key "a" has 3 tasks queued, key "b" has 2, submitted a,a,a,b,b: a
-- fully-draining scheduler would finish a1,a2,a3,b1,b2. The
-- least-served-first counter instead alternates: a1,b1,a2,b2,a3.

local scheduled = require('arken.concurrent.task.scheduled')
local mvm       = require('arken.mvm')
local worker    = 'support/concurrent_task/recorder_worker.lua'
local test      = {}

test.busiest_key_does_not_starve_the_other = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:scheduled:fairness:' .. tostring(os.microtime())

  scheduled.max(1)

  scheduled.start(worker, { bucket = bucket, tag = 'a1', sleep = 0.05 }, 'a')
  scheduled.start(worker, { bucket = bucket, tag = 'a2', sleep = 0.05 }, 'a')
  scheduled.start(worker, { bucket = bucket, tag = 'a3', sleep = 0.05 }, 'a')
  scheduled.start(worker, { bucket = bucket, tag = 'b1', sleep = 0.05 }, 'b')
  scheduled.start(worker, { bucket = bucket, tag = 'b2', sleep = 0.05 }, 'b')

  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 5, #intervals )
  timeline.assertNoOverlap(intervals)

  assert( intervals[1].tag == 'a1', intervals[1].tag )
  assert( intervals[2].tag == 'b1', intervals[2].tag )
  assert( intervals[3].tag == 'a2', intervals[3].tag )
  assert( intervals[4].tag == 'b2', intervals[4].tag )
  assert( intervals[5].tag == 'a3', intervals[5].tag )
end

return test
