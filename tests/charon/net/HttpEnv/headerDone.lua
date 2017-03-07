local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['first example'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:headerDone() == nil)
end

return test
