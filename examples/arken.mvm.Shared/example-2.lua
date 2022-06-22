local mvm  = require('arken.mvm')
local task = require('arken.concurrent.task')
local tsk  = task.start(os.pwd() .. '/examples/arken.mvm.Shared/example-2-node.lua', {}, false)

tsk:wait()

print("result => " .. tsk:shared():getString('hello'))
