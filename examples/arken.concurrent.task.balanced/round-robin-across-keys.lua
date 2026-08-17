-- arken.concurrent.task.balanced example: round-robin across keys
--
-- balanced.start(file, params, key, release) also keeps one queue per
-- key (map()/vector()), but dequeue() just cycles a position pointer
-- through the known keys in rotation, taking one item from whichever key
-- is next in line and dropping a key from rotation once its queue is
-- empty. There is no runners() counter here at all -- balanced does not
-- track "busy" or "how many times served" for a key in any way.
--
-- IMPORTANT: like scheduled, this is about distributing work evenly
-- across keys, NOT about exclusivity within a key. Two tasks for the
-- SAME key can run concurrently if the rotation cycles back to that key
-- before the previous task for it has finished. For "never overlap for
-- this key", use arken.concurrent.task.singular instead.
--
-- Two keys below get several tasks each: watch execution alternate "a",
-- "b", "a", "b", ... instead of draining one key before the other.

local balanced = require('arken.concurrent.task.balanced')
local worker    = 'examples/arken.concurrent.task.balanced/worker'

balanced.max(2)

balanced.start(worker, { key = 'a', i = 1, sleep = 0.2 }, 'a')
balanced.start(worker, { key = 'b', i = 1, sleep = 0.2 }, 'b')
balanced.start(worker, { key = 'a', i = 2, sleep = 0.2 }, 'a')
balanced.start(worker, { key = 'b', i = 2, sleep = 0.2 }, 'b')

require('arken.mvm').wait()
print('done')
