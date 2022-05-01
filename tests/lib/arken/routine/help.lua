local routine = require 'arken.routine'
local json = require 'arken.json'
local test = {}

-------------------------------------------------------------------------------
-- OUTPUT TEST

local result = ''

function output(value)
  result = value
end

routine.output = output

-------------------------------------------------------------------------------
-- MODULE TEST

local MyRoutineTask = {}
MyRoutineTask.help  = {}

MyRoutineTask.help.prepapre = [[
  prepare database for development
]]

MyRoutineTask.prepare = function()
end

test.should_return_string_help = function()
  routine.help(MyRoutineTask)
  assert( result == 'prepapre # prepare database for development\n', result )
end

return test
