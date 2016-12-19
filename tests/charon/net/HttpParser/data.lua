local HttpParser = require "charon.net.HttpParser"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header  = os.read(CHARON_PATH .. '/tests/charon/net/HttpParser/example-1.header')
  local request = HttpParser.new(header)
  assert(request:data() == header)
end

return test
