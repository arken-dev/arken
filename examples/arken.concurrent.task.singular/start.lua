-- arken.concurrent.task.singular example: start()
--
-- start(file, params, key) ALWAYS queues a new task for `key`, no matter
-- how many tasks for that key are already queued or running. Call it
-- three times in a row for the same key and you get three tasks queued,
-- executed one after another (see fifo-per-key.lua for the ordering
-- guarantee, and exclusive-per-key.lua for the "never overlap" one).
--
-- Use `start` when every call really means "one more unit of work", e.g.
-- one task per incoming webhook -- you WANT every event queued, none
-- silently dropped.
--
-- Compare with emplace.lua and place.lua, which skip the call under
-- certain conditions instead of always queuing.

local singular = require('arken.concurrent.task.singular')
local worker    = 'examples/arken.concurrent.task.singular/worker'
local key       = 'marketplace:1234'

-- three calls, three tasks queued -- always, unconditionally
singular.start(worker, { key = key, i = 1, sleep = 0.2 }, key)
singular.start(worker, { key = key, i = 2, sleep = 0.2 }, key)
singular.start(worker, { key = key, i = 3, sleep = 0.2 }, key)

require('arken.mvm').wait()
print('done')
