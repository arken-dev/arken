local HttpParser = require "charon.net.HttpParser"

local test = {}

test['first example'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpParser/example-1.header')
  local parser = HttpParser.new(header)
  assert(parser:fragment() == nil)
end

return test
