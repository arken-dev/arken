local HttpEnv = require "arken.net.HttpEnv"
local mvm  = require "arken.mvm"
local test = {}

test['first example'] = function()
  local header = os.read(mvm.path() .. '/tests/lib/arken/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:headerDone() == nil)
end

test['should set headerDone'] = function()
  local parser = HttpEnv.new("")
  parser:setHeaderDone("test header done")
  assert(parser:headerDone() == "test header done", parser:headerDone())
end

return test
