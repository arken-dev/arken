-- arken.concurrent.task.priority example: higher priority runs first
--
-- priority.start(file, params, priority, release) also has NO key/name
-- parameter -- one shared queue, but backed by a std::priority_queue
-- instead of a plain FIFO queue. Straight from priority.cpp's comparator:
-- a HIGHER `priority` value is dequeued first; ties are broken by
-- submission order (the one submitted earlier wins the tie).
--
-- With max(1), submitting low-priority work first and high-priority work
-- right after still runs the high-priority one first -- unlike fifo,
-- where submission order alone would decide.
--
-- There is no key here either: priority reorders ONE global queue, it
-- does not give per-resource exclusivity (see arken.concurrent.task.
-- singular for that).

local priority = require('arken.concurrent.task.priority')

priority.max(1)

-- submitted first, but LOW priority
priority.start('examples/arken.concurrent.task.priority/worker', { i = 'low',    priority = 1, sleep = 0.2 }, 1)
-- submitted right after, HIGH priority -- runs before "low" even though
-- it was queued second
priority.start('examples/arken.concurrent.task.priority/worker', { i = 'high',   priority = 9, sleep = 0.2 }, 9)
-- middle priority, runs between the two above
priority.start('examples/arken.concurrent.task.priority/worker', { i = 'medium', priority = 5, sleep = 0.2 }, 5)

require('arken.mvm').wait()
print('done')
