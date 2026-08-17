-- arken.concurrent.task.fifo: one global queue, strict submission order
--
-- No key/name parameter -- every task started through this module shares
-- ONE queue (fifo_queue()). With max(1), tasks are dequeued and finish in
-- exactly the order they were submitted.

local fifo   = require('arken.concurrent.task.fifo')
local mvm    = require('arken.mvm')
local worker = 'support/concurrent_task/recorder_worker.lua'
local test   = {}

test.tasks_finish_in_submission_order = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:fifo:order:' .. tostring(os.microtime())

  fifo.max(1)

  fifo.start(worker, { bucket = bucket, tag = 'a', sleep = 0.05 })
  fifo.start(worker, { bucket = bucket, tag = 'b', sleep = 0.05 })
  fifo.start(worker, { bucket = bucket, tag = 'c', sleep = 0.05 })
  fifo.start(worker, { bucket = bucket, tag = 'd', sleep = 0.05 })

  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 4, #intervals )
  timeline.assertNoOverlap(intervals)

  assert( intervals[1].tag == 'a', intervals[1].tag )
  assert( intervals[2].tag == 'b', intervals[2].tag )
  assert( intervals[3].tag == 'c', intervals[3].tag )
  assert( intervals[4].tag == 'd', intervals[4].tag )
end

return test
