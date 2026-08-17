-- arken.concurrent.task.singular example: node:wait()
--
-- singular.start/emplace/place return a node. node:wait() blocks the
-- calling task until THAT SPECIFIC task finishes -- it does not affect
-- the serialization of other tasks that share (or do not share) its key.

local singular = require('arken.concurrent.task.singular')

local node = singular.start(
  'examples/arken.concurrent.task.singular/worker',
  { key = 'marketplace:1234', i = 1, sleep = 0.5 },
  'marketplace:1234'
)

print('waiting for the task to finish...')
node:wait()
print('task finished, node:finished() = ' .. tostring(node:finished()))
