-- arken.concurrent.task.singular example: FIFO order per key
--
-- Tasks queued under the same key run in the order they were submitted --
-- singular keeps one queue per key (map()[key]) and only dequeues the
-- next item once the previous one has finished. Submit A, B, C for the
-- same key and they execute as A, then B, then C -- never B before A.

local singular = require('arken.concurrent.task.singular')

local key = 'marketplace:1234'

singular.start('examples/arken.concurrent.task.singular/worker.lua', { key = key, i = 'A', sleep = 0.3 }, key)
singular.start('examples/arken.concurrent.task.singular/worker.lua', { key = key, i = 'B', sleep = 0.3 }, key)
local c = singular.start('examples/arken.concurrent.task.singular/worker.lua', { key = key, i = 'C', sleep = 0.3 }, key)

-- Waiting on the LAST task queued is enough: it can only finish after A
-- and B, since all three share the same key and run strictly in order.
c:wait()
print('done')
