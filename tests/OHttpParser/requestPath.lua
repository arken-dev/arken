require "OHttpParser"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(CHARON_PATH .. '/tests/OHttpParser/example-1.header')
  local parser = OHttpParser.new(header)
  assert(parser:requestPath() == '/pedido/varejo')
end

return test
