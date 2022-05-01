local HttpEnv = require "arken.net.HttpEnv"
local mvm  = require "arken.mvm"
local test = {}

test['should return /pedido/varejo'] = function()
  local header = os.read(mvm.path() .. '/tests/lib/arken/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:requestUri() == '/pedido/varejo?id=1234&descricao=teste')
end

test['should define uri'] = function()
  local parser = HttpEnv.new("")
  parser:setRequestUri("/home/order?id=999")
  assert(parser:requestUri() == "/home/order?id=999", parser:requestUri())
end

return test
