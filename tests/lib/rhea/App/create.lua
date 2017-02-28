local test = {}
local rhea = require('rhea')
local App  = require('rhea.App')

App.output = function(value)
end

test.should_copy_of_skel = function()
  local dir    = os.temp() .. '/' .. os.uuid()
  local params = {}
  params[0] = 'app:create'
  params[1] = dir

  rhea.run(params)
  assert( os.exists(dir) == true )
  assert( os.exists(dir .. '/app') == true )
  assert( os.exists(dir .. '/config/active_record.json') == true )
end

test.should_error_if_dir_exists = function()
  local dir    = os.temp() .. '/' .. os.uuid()
  local params = {}
  params[0] = 'app:create'
  params[1] = dir

  os.mkdir(dir)
  local status, message = pcall(rhea.run, params)

  assert( status == false )
  assert( message:contains('exists') == true )
end

return test
