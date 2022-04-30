local HttpEnv = require "arken.net.HttpEnv"
local mvm  = require "arken.mvm"
local test = {}

test['first example'] = function()
  local header = os.read(mvm.path() .. '/tests/arken/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:fragment() == nil)
end

test['should define fragment'] = function()
  local parser = HttpEnv.new("")
  parser:setFragment("teste fragment")
  assert( parser:fragment() == "teste fragment", parser:fragment() )
end

return test
