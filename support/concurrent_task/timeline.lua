-- Shared by the concurrent/task test suites (singular, fifo, priority,
-- scheduled, balanced): turns the event log written by
-- _recorder_worker.lua into an ordered timeline. Loaded with dofile (not
-- require) so each test gets a fresh copy and there is no module-name
-- mangling to worry about, matching how task.start() already loads
-- worker files in this test tree.
--
-- bucket.keys() order is not the write order, so events are re-sorted by
-- the numeric "<microtime>#<rand>" prefix each key was written with.

local cache = require('arken.cache')

local M = {}

function M.events(bucketName)
  local bucket = cache.bucket(bucketName)
  local events = {}

  for _, key in ipairs(bucket.keys()) do
    local value     = bucket.get(key)
    local time      = tonumber(key:match('^([^#]+)#'))
    local kind, tag = value:match('^(%a+):(.+)$')
    table.insert(events, { time = time, kind = kind, tag = tag })
  end

  table.sort(events, function(a, b) return a.time < b.time end)
  return events
end

-- pairs up each "start:<tag>" with the next "finish:<tag>" for the same
-- tag -- callers must use a fresh tag per spawned task so the pairing is
-- unambiguous. Returned in the order each pair was closed (finish time).
function M.intervals(bucketName)
  local pending   = {}
  local intervals = {}

  for _, event in ipairs(M.events(bucketName)) do
    if event.kind == 'start' then
      pending[event.tag] = event.time
    elseif event.kind == 'finish' then
      table.insert(intervals, { tag = event.tag, start = pending[event.tag], finish = event.time })
      pending[event.tag] = nil
    end
  end

  return intervals
end

function M.overlaps(a, b)
  return a.start < b.finish and b.start < a.finish
end

function M.assertNoOverlap(intervals)
  for i = 1, #intervals do
    for j = i + 1, #intervals do
      if M.overlaps(intervals[i], intervals[j]) then
        error(string.format('intervals overlap: %s [%s..%s] and %s [%s..%s]',
          intervals[i].tag, intervals[i].start, intervals[i].finish,
          intervals[j].tag, intervals[j].start, intervals[j].finish))
      end
    end
  end
end

return M
