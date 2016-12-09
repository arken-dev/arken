require "CHttpParser"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-1.header')
  local parser = CHttpParser.new(header)
  assert(parser:requestUri() == '/pedido/varejo?id=1234&descricao=teste')
end

return test
