require "OHttpParser"
local should = require "test.should"
local test   = {}

test['deve retornar path /pedido/varejo'] = function()
  local header  = os.read(OBERON_PATH .. '/specs/OHttpParser/example-1.header')
  local request = OHttpParser.new(header)
  should.be_equal(request:data(), header)
end

return test
