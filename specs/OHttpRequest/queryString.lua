require "OHttpRequest"
local should = require "test.should"
local test   = {}

test['deve retornar path id=1234&descricao=teste'] = function()
  local header = os.read(OBERON_PATH .. '/specs/OHttpRequest/example-1.header')
  local parser = OHttpRequest.new(header)
  should.be_equal(parser:queryString(), 'id=1234&descricao=teste')
end

return test
