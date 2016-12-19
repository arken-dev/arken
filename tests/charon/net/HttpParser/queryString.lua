local HttpParser = require "charon.net.HttpParser"

local test   = {}

test['deve retornar path id=1234&descricao=teste'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpParser/example-1.header')
  local parser = HttpParser.new(header)
  assert(parser:queryString() == 'id=1234&descricao=teste')
end

return test
