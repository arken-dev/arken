-- arken.concurrent.task.singular example: place()
--
-- place(file, params, key) queues a new task for `key` ONLY IF there is
-- no runner tracked for `key` at all yet. Checked condition, straight
-- from singular.cpp:
--
--   runners().count(key) == 0
--
-- That entry is created the first time `key` is used and only erased
-- once `key`'s queue fully drains AND the last task for it finishes --
-- so place() blocks new work for `key` both while a task is QUEUED and
-- while one is RUNNING. This is a stricter "one in flight, period" gate
-- than emplace(), which only looks at whether the queue is empty. When
-- place() decides not to queue, it returns an empty node.
--
-- Use `place` for "exactly one attempt in flight for this key, full
-- stop" -- e.g. a marketplace token refresh that must not be triggered
-- again while a previous refresh (queued OR running) for the same
-- marketplace hasn't finished yet.
--
-- Compare with start.lua (always queues) and emplace.lua (weaker: only
-- blocks while the queue itself is non-empty).

local singular = require('arken.concurrent.task.singular')
local worker    = 'examples/arken.concurrent.task.singular/worker.lua'
local key       = 'marketplace:1234'

singular.place(worker, { key = key, i = 1, sleep = 0.3 }, key)

-- called again immediately: a runner is still tracked for `key` (queued
-- or running), so this call is skipped -- no second task gets queued.
singular.place(worker, { key = key, i = 'place (skipped)', sleep = 0.3 }, key)

require('arken.mvm').wait()
print('done')
