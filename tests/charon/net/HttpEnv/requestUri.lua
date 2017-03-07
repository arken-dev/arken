local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:requestUri() == '/pedido/varejo?id=1234&descricao=teste')
end

return test
