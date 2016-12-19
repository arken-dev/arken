local HttpParser = require "charon.net.HttpParser"

local test = {}

test['first example'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpParser/example-1.header')
  local parser = HttpParser.new(header)
  assert(parser:field("User-Agent") == "Mozilla/5.0 (X11; Linux x86_64; rv:38.0)")
end

return test
