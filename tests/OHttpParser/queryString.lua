require "OHttpParser"

local test   = {}

test['deve retornar path id=1234&descricao=teste'] = function()
  local header = os.read(OBERON_PATH .. '/tests/OHttpParser/example-1.header')
  local parser = OHttpParser.new(header)
  assert(parser:queryString() == 'id=1234&descricao=teste')
end

return test
