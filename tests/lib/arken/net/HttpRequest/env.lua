local HttpRequest = require('arken.net.HttpRequest')
local HttpEnv = require('arken.net.HttpEnv')
local mvm = require('arken.mvm')
local test = {}

test.should_return_env_instance = function()
  local header = os.read(mvm.path() .. '/tests/lib/arken/net/HttpEnv/example-2.txt')
  local env = HttpEnv.new(header)
  local request = HttpRequest.new{ _env = env }
  assert( request:env() == env )
end

return test
