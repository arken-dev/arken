-- arken.concurrent.task.singular example: exclusivity per key
--
-- singular.start(fileName, params, key) queues work under `key`. The
-- runners()/map() bookkeeping inside singular guarantees that at most one
-- task per key executes at a time, no matter how many worker threads the
-- process has -- a second task started under the SAME key while the first
-- is still running gets queued and only runs after the first one
-- finishes. Tasks under DIFFERENT keys are completely independent and can
-- run concurrently.
--
-- This is the mechanism to reach for whenever two operations touch the
-- same logical resource and must never overlap -- e.g. a webhook and a
-- reconciliation job both acting on "marketplace:1234", or any other
-- "one thing at a time for this id" requirement. Key formats like
-- "marketplace:<id>" or "customer:<id>" are just plain strings: singular
-- does not parse them, it only compares them for equality.

local singular = require('arken.concurrent.task.singular')
local worker    = 'examples/arken.concurrent.task.singular/worker'

-- Three tasks under the SAME key: they run one after another, never
-- overlapping -- watch the [worker] start/finish lines for
-- "marketplace:1234" interleave in strict pairs.
singular.start(worker, { key = 'marketplace:1234', i = 1, sleep = 0.3 }, 'marketplace:1234')
singular.start(worker, { key = 'marketplace:1234', i = 2, sleep = 0.3 }, 'marketplace:1234')
singular.start(worker, { key = 'marketplace:1234', i = 3, sleep = 0.3 }, 'marketplace:1234')

-- A task under a DIFFERENT key is independent from marketplace:1234: it
-- can start and finish while the tasks above are still queued/running.
singular.start(worker, { key = 'marketplace:5678', i = 1, sleep = 0.3 }, 'marketplace:5678')

require('arken.mvm').wait()
print('done')
