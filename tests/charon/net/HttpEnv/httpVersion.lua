local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:httpVersion() == 'HTTP/1.1')
end

return test
