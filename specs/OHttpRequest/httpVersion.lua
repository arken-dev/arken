require "OHttpRequest"
local should = require "test.should"
local test   = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(OBERON_PATH .. '/specs/OHttpRequest/example-1.header')
  local parser = OHttpRequest.new(header)
  should.be_equal(parser:httpVersion(), 'HTTP/1.1')
end

return test
