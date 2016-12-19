local HttpParser = require "charon.net.HttpParser"

local test = {}

test['deve retornar GET'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpParser/example-1.header')
  local parser = HttpParser.new(header)
  assert(parser:requestMethod() == 'GET')
end

return test
