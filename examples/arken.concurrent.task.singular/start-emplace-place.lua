-- arken.concurrent.task.singular example: start vs emplace vs place
--
-- The three constructors are NOT interchangeable -- they differ in what
-- happens when you call them again for a key that already has work
-- queued or running:
--
--   start(file, params, key)   -- always queues a new task for `key`,
--                                  no matter what.
--
--   emplace(file, params, key) -- queues a new task ONLY IF `key`'s queue
--                                  is currently empty. If a task for this
--                                  key is already queued (even if none is
--                                  running yet), the call is a no-op.
--
--   place(file, params, key)   -- queues a new task ONLY IF there is no
--                                  runner at all tracked for `key` yet --
--                                  a stricter "one in flight" gate than
--                                  emplace, cleared only once that key's
--                                  queue fully drains.
--
-- Pick `start` for "always run this", `emplace`/`place` for "run this
-- unless one is already pending/running for this key" -- e.g. coalescing
-- repeated reconciliation triggers for the same marketplace into a
-- single pass instead of piling up duplicates.

local singular = require('arken.concurrent.task.singular')

local key = 'marketplace:1234'

-- start: two calls, two tasks queued.
singular.start('examples/arken.concurrent.task.singular/worker', { key = key, i = 1, sleep = 0.2 }, key)
singular.start('examples/arken.concurrent.task.singular/worker', { key = key, i = 2, sleep = 0.2 }, key)

-- emplace: called again immediately for the same key, while the queue
-- from the calls above is still non-empty -- this one is skipped.
singular.emplace(
  'examples/arken.concurrent.task.singular/worker',
  { key = key, i = 'emplace (likely skipped)', sleep = 0.2 },
  key
)

require('arken.mvm').wait()
print('done')
