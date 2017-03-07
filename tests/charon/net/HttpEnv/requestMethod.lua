local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['deve retornar GET'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:requestMethod() == 'GET')
end

return test
