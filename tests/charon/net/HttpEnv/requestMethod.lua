local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['should return GET'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:requestMethod() == 'GET')
end

test['show define requestMethod'] = function()
  local parser = HttpEnv.new("")
  parser:setRequestMethod("PUT")
  assert(parser:requestMethod() == 'PUT', parser:requestMethod())
end

return test
