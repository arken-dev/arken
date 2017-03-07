local HttpRequest = require('charon.net.HttpRequest')
local HttpEnv = require('charon.net.HttpEnv')
local test = {}

test.should_return_env_instance = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-2.txt')
  local env = HttpEnv.new(header)
  local request = HttpRequest.new{ _env = env }
  assert( request:env() == env )
end

return test
