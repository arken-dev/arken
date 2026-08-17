-- Shared worker used by the concurrent/task test suites (singular, fifo,
-- priority, scheduled, balanced). Each spawned task file runs in its own
-- Lua VM (mvm::getInstance), so there is no shared upvalue between the
-- test and this worker -- params.bucket names an arken.cache bucket
-- (process-wide, visible across VMs) used as a simple event log:
-- "start:<tag>" is recorded, then a sleep to simulate work, then
-- "finish:<tag>". Each record uses its OWN unique key (timestamp +
-- random suffix), so there is no read-modify-write race on the write
-- itself.

return function(node, params)
  local cache  = require('arken.cache')
  local bucket = cache.bucket(params.bucket)

  bucket.put(tostring(os.microtime()) .. '#' .. tostring(math.random(1, 1000000000)), 'start:' .. params.tag)
  os.sleep(params.sleep or 0.05)
  bucket.put(tostring(os.microtime()) .. '#' .. tostring(math.random(1, 1000000000)), 'finish:' .. params.tag)
end
