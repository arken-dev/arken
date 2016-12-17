require "CHttpParser"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-1.header')
  local parser = CHttpParser.new(header)
  assert(parser:requestPath() == '/pedido/varejo')
end

return test
