local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['first example'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:headerDone() == nil)
end

test['should set headerDone'] = function()
  local parser = HttpEnv.new("")
  parser:setHeaderDone("test header done")
  assert(parser:headerDone() == "test header done", parser:headerDone())
end

return test
