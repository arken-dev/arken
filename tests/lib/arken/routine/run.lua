local routine = require 'arken.routine'
local json = require 'arken.json'
local mvm  = require 'arken.mvm'
local test = {}
local MyRoutineTask = require('util.routine.MyRoutineTask')

test.beforeAll = function()
  package.path = package.path .. ';' .. mvm.path() .. '/util/?.lua'
  routine.path = package.path
end

test.afterAll = function()
  package.path = package.path:mid(1, routine.path:lastIndexOf(';')-1)
end

-------------------------------------------------------------------------------
-- OUTPUT TEST

local result = ''

function output(value)
  result = value
end

routine.output = output

-------------------------------------------------------------------------------
-- MODULE TEST
--[[
test.should_return_string_help = function()
  routine.run({})
  assert( result == 'prepare # prepare database for development\n', result )
end

test.should_execute_task = function()
  local params = {}
  params[0] = 'my_routine_task:prepare'
  assert( MyRoutineTask.flag == false )
  routine.run(params)
  assert( MyRoutineTask.flag == true )
end

test.should_execute_help_undocumentad = function()
  local params = {}
  params[0] = 'my_routine_task:other'
  params[1] = '--help'
  routine.run(params)
  assert( result == 'other: undocumented', result )
end

test.should_execute_help_action = function()
  local params = {}
  params[0] = 'my_routine_task:prepare'
  params[1] = '--help'
  routine.run(params)
  assert( result == 'prepare # prepare database for development', result )
end

test.should_execute_help_module = function()
  local params = {}
  params[0] = 'my_routine_task'
  params[1] = '--help'
  routine.run(params)
  assert( result == 'prepare # prepare database for development\n', result )
end

test.should_error_if_module_not_load = function()
  local params = {}
  params[0] = 'my_routine_task'
  params[1] = '--help'
  routine.run(params)
  assert( result == 'prepare # prepare database for development\n', result )
end
]]
return test
