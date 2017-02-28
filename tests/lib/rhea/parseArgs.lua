local rhea = require 'rhea'
local json = require('charon.json')
local test = {}

test.should_return_params_with_dahed = function()
  local args   = {'migrate:run', '--param1', 'value1'}
  local result = rhea.parseArgs(args)
  assert( type(result.param1) == 'string', json.encode(result) )
  assert( result.param1 == 'value1', json.encode(result) )
end

test.should_return_multiple_params_with_dahed = function()
  local args   = {'migrate:run', '--param1', 'value1', 'value2', 'value3'}
  local result = rhea.parseArgs(args)
  assert( type(result.param1) == 'table', json.encode(result) )
  assert( result.param1[1] == 'value1', json.encode(result) )
  assert( result.param1[2] == 'value2', json.encode(result) )
  assert( result.param1[3] == 'value3', json.encode(result) )
end

test.should_return_true_if_wihtout_value_for_dahed_param = function()
  local args   = {'migrate:run', '--param1'}
  local result = rhea.parseArgs(args)
  assert( type(result.param1) == 'boolean', json.encode(result) )
  assert( result.param1 == true, json.encode(result) )
end

test.should_return_multiple_params = function()
  local args   = {'migrate:run', '--param1', 'value1', '--param2', 'value2'}
  local result = rhea.parseArgs(args)
  assert( type(result.param1) == 'string', json.encode(result) )
  assert( result.param1 == 'value1', json.encode(result) )
  assert( type(result.param2) == 'string', json.encode(result) )
  assert( result.param2 == 'value2', json.encode(result) )
end

test.should_return_first_boolean_and_last_string = function()
  local args   = {'migrate:run', '--param1', '--param2', 'value2'}
  local result = rhea.parseArgs(args)
  assert( type(result.param1) == 'boolean', json.encode(result) )
  assert( result.param1 == true, json.encode(result) )
  assert( type(result.param2) == 'string', json.encode(result) )
  assert( result.param2 == 'value2', json.encode(result) )
end

test.should_return_first_value_second_boolean_and_last_string = function()
  local args   = {'migrate:run', '--param1', 'value1', 'value2', '--param2', '--param3', 'value3'}
  local result = rhea.parseArgs(args)
  assert( type(result.param1) == 'table', json.encode(result) )
  assert( result.param1[1] == 'value1', json.encode(result) )
  assert( result.param1[2] == 'value2', json.encode(result) )
  assert( type(result.param2) == 'boolean', json.encode(result) )
  assert( result.param2 == true, json.encode(result) )
  assert( type(result.param3) == 'string', json.encode(result) )
  assert( result.param3 == 'value3', json.encode(result) )
end

test.should_return_numeric_index = function()
  local args   = {}
  args[0] = 'migrate:run'
  args[1] = '/tmp/appdir'
  local result = rhea.parseArgs(args)
  assert(result[1] == '/tmp/appdir', json.encode(result))
end

return test
