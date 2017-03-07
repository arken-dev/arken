local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header  = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local request = HttpEnv.new(header)
  assert(request:data() == header)
end

return test
