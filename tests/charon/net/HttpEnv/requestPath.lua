local HttpEnv = require "arken.net.HttpEnv"

local test = {}

test['should return /pedido/varejo'] = function()
  local header = os.read(ARKEN_PATH .. '/tests/arken/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:requestPath() == '/pedido/varejo')
end

test['should define requestPath'] = function()
  local parser = HttpEnv.new("")
  parser:setRequestPath("/home/order")
  assert(parser:requestPath() == '/home/order', parser:requestPath())
end

return test
