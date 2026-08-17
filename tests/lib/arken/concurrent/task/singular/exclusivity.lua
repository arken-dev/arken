-- arken.concurrent.task.singular: exclusivity per key
--
-- Uses start() (always queues) so both tests are purely about how the
-- module SCHEDULES the queued work, not about start()/emplace()/place()
-- skip conditions (see emplace.lua and place.lua for those).

local singular = require('arken.concurrent.task.singular')
local mvm      = require('arken.mvm')
local worker   = 'support/concurrent_task/recorder_worker.lua'
local test     = {}

test.same_key_never_runs_two_tasks_at_once = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:singular:exclusivity:' .. tostring(os.microtime())
  local key      = 'account:42'

  singular.start(worker, { bucket = bucket, tag = 'a', sleep = 0.1 }, key)
  singular.start(worker, { bucket = bucket, tag = 'b', sleep = 0.1 }, key)
  singular.start(worker, { bucket = bucket, tag = 'c', sleep = 0.1 }, key)

  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 3, #intervals )
  timeline.assertNoOverlap(intervals)

  -- same-key queue is FIFO: closed in submission order
  assert( intervals[1].tag == 'a', intervals[1].tag )
  assert( intervals[2].tag == 'b', intervals[2].tag )
  assert( intervals[3].tag == 'c', intervals[3].tag )
end

test.different_keys_run_in_parallel = function()
  local timeline = dofile('support/concurrent_task/timeline.lua')
  local bucket   = 'test:singular:parallel:' .. tostring(os.microtime())

  singular.max(2)

  singular.start(worker, { bucket = bucket, tag = 'a', sleep = 0.2 }, 'account:1')
  singular.start(worker, { bucket = bucket, tag = 'b', sleep = 0.2 }, 'account:2')

  mvm.wait()

  local intervals = timeline.intervals(bucket)
  assert( #intervals == 2, #intervals )

  local a, b
  for _, interval in ipairs(intervals) do
    if interval.tag == 'a' then a = interval end
    if interval.tag == 'b' then b = interval end
  end

  assert( timeline.overlaps(a, b), 'expected tasks for different keys to overlap, they ran fully sequentially' )
end

return test
