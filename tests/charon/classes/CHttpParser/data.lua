require "CHttpParser"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header  = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-1.header')
  local request = CHttpParser.new(header)
  assert(request:data() == header)
end

return test
