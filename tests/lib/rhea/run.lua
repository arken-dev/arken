local rhea = require 'rhea'
local json = require 'arken.json'
local test = {}
local MyRheaTask = require('util.rhea.MyRheaTask')

test.beforeAll = function()
  package.path = package.path .. ';' .. ARKEN_PATH .. '/util/?.lua'
  rhea.path = package.path
end

test.afterAll = function()
  package.path = package.path:mid(1, rhea.path:lastIndexOf(';')-1)
end

-------------------------------------------------------------------------------
-- OUTPUT TEST

local result = ''

function output(value)
  result = value
end

rhea.output = output

-------------------------------------------------------------------------------
-- MODULE TEST

test.should_return_string_help = function()
  rhea.run({})
  assert( result == 'prepare # prepare database for development\n', result )
end

test.should_execute_task = function()
  local params = {}
  params[0] = 'my_rhea_task:prepare'
  assert( MyRheaTask.flag == false )
  rhea.run(params)
  assert( MyRheaTask.flag == true )
end

test.should_execute_help_undocumentad = function()
  local params = {}
  params[0] = 'my_rhea_task:other'
  params[1] = '--help'
  rhea.run(params)
  assert( result == 'other: undocumented', result )
end

test.should_execute_help_action = function()
  local params = {}
  params[0] = 'my_rhea_task:prepare'
  params[1] = '--help'
  rhea.run(params)
  assert( result == 'prepare # prepare database for development', result )
end

test.should_execute_help_module = function()
  local params = {}
  params[0] = 'my_rhea_task'
  params[1] = '--help'
  rhea.run(params)
  assert( result == 'prepare # prepare database for development\n', result )
end

test.should_error_if_module_not_load = function()
  local params = {}
  params[0] = 'my_rhea_task'
  params[1] = '--help'
  rhea.run(params)
  assert( result == 'prepare # prepare database for development\n', result )
end


return test
