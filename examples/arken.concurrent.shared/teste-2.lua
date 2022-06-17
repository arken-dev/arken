local mvm  = require('arken.mvm')
local task = require('arken.concurrent.task')
local tsk  = task.start(os.pwd() .. '/examples/arken.concurrent.shared/outro.lua', {}, false)

mvm.wait()

print("resultado => " .. tsk:shared():getString('hello'))
