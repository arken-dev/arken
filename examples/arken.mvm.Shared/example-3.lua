local mvm  = require('arken.mvm')
local task = require('arken.concurrent.task')
local tsk  = task.start(os.pwd() .. '/examples/arken.mvm.Shared/example-3-node.lua', {}, false)

mvm.wait()

print("result => " .. tsk:shared():get('hello'))
