require "OHttpParser"
local should = require "test.should"
local test   = {}

test['deve retornar path /pedido/varejo'] = function()
  local header = os.read(OBERON_PATH .. '/specs/OHttpParser/example-1.header')
  local parser = OHttpParser.new(header)
  should.be_equal(parser:requestPath(), '/pedido/varejo')
end

return test
