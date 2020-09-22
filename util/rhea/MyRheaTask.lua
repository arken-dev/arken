local Class = require('arken.oop.Class')
local MyRheaTask = Class.new("MyRheaTask")
MyRheaTask.help  = {}

MyRheaTask.help.prepare = [[
  prepare database for development
]]

MyRheaTask.flag = false

function MyRheaTask:prepare()
  MyRheaTask.flag = true
end

function MyRheaTask:other()
end

return MyRheaTask
