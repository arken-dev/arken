require "OHttpParser"

local test = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(OBERON_PATH .. '/tests/OHttpParser/example-1.header')
  local parser = OHttpParser.new(header)
  assert(parser:httpVersion() == 'HTTP/1.1')
end

return test
