require "OHttpRequest"
local should = require "test.should"
local test   = {}

test['first example'] = function()
  local header = os.read(OBERON_PATH .. '/specs/OHttpRequest/example-1.header')
  local parser = OHttpRequest.new(header)
  should.be_equal(parser:fragment(), nil)
end

return test
