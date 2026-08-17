-- arken.concurrent.task.balanced: round-robin across keys
--
-- Like scheduled, one queue per key (map()/vector()), but dequeue() just
-- cycles a position pointer through the known keys in rotation -- no
-- runners() counter at all, a key is dropped from rotation only once its
-- own queue is found empty. This is NOT exclusivity: two tasks for the
-- same key can run concurrently if the rotation cycles back before the
-- previous one finished (see singular for that guarantee). With max(1)
-- there is only one worker, isolating pure ordering.
--
-- key "a" has 3 tasks queued, key "b" has 2, submitted a,a,a,b,b: a
-- fully-draining scheduler would finish a1,a2,a3,b1,b2. Pure rotation
-- instead alternates: a1,b1,a2,b2,a3.

local balanced = require('arken.concurrent.task.balanced')
local mvm      = require('arken.mvm')
local worker   = 'support/concurrent_task/recorder_worker.lua'
local test     = {}

test.rotation_alternates_between_keys = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:balanced:round-robin:' .. tostring(os.microtime())

  balanced.max(1)

  balanced.start(worker, { bucket = bucket, tag = 'a1', sleep = 0.05 }, 'a')
  balanced.start(worker, { bucket = bucket, tag = 'a2', sleep = 0.05 }, 'a')
  balanced.start(worker, { bucket = bucket, tag = 'a3', sleep = 0.05 }, 'a')
  balanced.start(worker, { bucket = bucket, tag = 'b1', sleep = 0.05 }, 'b')
  balanced.start(worker, { bucket = bucket, tag = 'b2', sleep = 0.05 }, 'b')

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
