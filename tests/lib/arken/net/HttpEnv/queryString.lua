local HttpEnv = require "arken.net.HttpEnv"
local mvm  = require "arken.mvm"
local test = {}

test['should return queryString id=1234&descricao=teste'] = function()
  local header = os.read(mvm.path() .. '/tests/lib/arken/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:queryString() == 'id=1234&descricao=teste')
end

test['should define query string'] = function()
  local parser = HttpEnv.new("")
  parser:setQueryString("id=999")
  assert(parser:queryString() == 'id=999', parser:queryString())
end

return test
