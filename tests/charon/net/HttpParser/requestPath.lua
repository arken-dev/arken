local HttpParser = require "charon.net.HttpParser"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpParser/example-1.header')
  local parser = HttpParser.new(header)
  assert(parser:requestPath() == '/pedido/varejo')
end

return test
