-- arken.concurrent.task.scheduled example: fairness across keys
--
-- scheduled.start(file, params, key, release) looks structurally like
-- singular -- a map() of queues and a runners() table keyed by `key` --
-- but runners() here is a COUNTER, not a busy/idle flag. dequeue() always
-- picks the key with the FEWEST runners() so far among the keys that
-- currently have work, so no single key can starve the others when many
-- keys are competing for the same worker pool.
--
-- IMPORTANT: this is a fairness mechanism ACROSS keys, not an
-- exclusivity mechanism WITHIN a key. Nothing stops two tasks for the
-- SAME key from being dequeued and running at the same time -- the
-- counter only influences which key gets picked next, it is never used
-- to block a key while one of its tasks is already running. If you need
-- "never overlap for this key", use arken.concurrent.task.singular
-- instead; scheduled solves a different problem (even service across
-- many keys).
--
-- Two keys below get several tasks each: watch how execution alternates
-- between "a" and "b" rather than draining one key's queue before
-- touching the other.

local scheduled = require('arken.concurrent.task.scheduled')
local worker     = 'examples/arken.concurrent.task.scheduled/worker'

scheduled.max(2)

scheduled.start(worker, { key = 'a', i = 1, sleep = 0.2 }, 'a')
scheduled.start(worker, { key = 'a', i = 2, sleep = 0.2 }, 'a')
scheduled.start(worker, { key = 'a', i = 3, sleep = 0.2 }, 'a')
scheduled.start(worker, { key = 'b', i = 1, sleep = 0.2 }, 'b')
scheduled.start(worker, { key = 'b', i = 2, sleep = 0.2 }, 'b')

require('arken.mvm').wait()
print('done')
