-- arken.concurrent.task.singular example: emplace()
--
-- emplace(file, params, key) queues a new task for `key` ONLY IF that
-- key's queue is currently EMPTY (never used, or nothing waiting right
-- now). Checked condition, straight from singular.cpp:
--
--   map().count(key) == 0  OR  map()[key].empty()
--
-- Note this only looks at the QUEUE, not whether a task for `key` is
-- currently RUNNING: if the previous task has already been dequeued (so
-- the queue is empty again) but hasn't finished yet, a new emplace() call
-- WILL still queue -- unlike place() below, which also blocks while one
-- is running. When emplace() decides not to queue, it returns an empty
-- node.
--
-- Use `emplace` to coalesce repeated triggers into "at most one pending"
-- per key -- e.g. multiple reconciliation webhooks arriving for the same
-- marketplace while one reconciliation is already queued: no need to
-- pile up duplicates that would just redo the same work.
--
-- Compare with start.lua (always queues) and place.lua (stricter: also
-- blocks while a task for `key` is running, not just while queued).

local singular = require('arken.concurrent.task.singular')
local worker    = 'examples/arken.concurrent.task.singular/worker'
local key       = 'marketplace:1234'

singular.start(worker, { key = key, i = 1, sleep = 0.3 }, key)

-- called again immediately, while the task above is still queued/running
-- (its queue slot is non-empty) -- this one is skipped, no second task
-- gets queued for `key`.
singular.emplace(worker, { key = key, i = 'emplace (skipped)', sleep = 0.3 }, key)

require('arken.mvm').wait()
print('done')
