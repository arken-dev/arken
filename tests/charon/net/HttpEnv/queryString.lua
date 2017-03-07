local HttpEnv = require "charon.net.HttpEnv"

local test   = {}

test['deve retornar path id=1234&descricao=teste'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:queryString() == 'id=1234&descricao=teste')
end

return test
