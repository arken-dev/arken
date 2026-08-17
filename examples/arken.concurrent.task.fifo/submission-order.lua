-- arken.concurrent.task.fifo example: one global queue, strict order
--
-- fifo.start(file, params, release) has NO key/name parameter -- unlike
-- singular/scheduled/balanced, there is only ONE queue shared by every
-- task started through this module (fifo_queue()). Tasks run in exactly
-- the order they were submitted.
--
-- max() controls how many tasks may run AT THE SAME TIME (workers pulling
-- from the single queue), not the order -- with max(1) the run below
-- prints strictly A, B, C, D, one fully finishing before the next starts.
-- Raise max() and several can execute concurrently while still being
-- DEQUEUED in submission order.
--
-- Use fifo when work items are interchangeable and only submission order
-- matters -- there is no notion of "same key" here, so it is not the
-- right tool for per-resource exclusivity (see arken.concurrent.task.
-- singular for that).

local fifo = require('arken.concurrent.task.fifo')

fifo.max(1)

fifo.start('examples/arken.concurrent.task.fifo/worker', { i = 'A', sleep = 0.2 })
fifo.start('examples/arken.concurrent.task.fifo/worker', { i = 'B', sleep = 0.2 })
fifo.start('examples/arken.concurrent.task.fifo/worker', { i = 'C', sleep = 0.2 })
fifo.start('examples/arken.concurrent.task.fifo/worker', { i = 'D', sleep = 0.2 })

require('arken.mvm').wait()
print('done')
