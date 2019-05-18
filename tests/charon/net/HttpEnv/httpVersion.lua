local HttpEnv = require "charon.net.HttpEnv"

local test = {}

test['should return path /pedido/varejo'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:httpVersion() == 'HTTP/1.1')
end

test['should define http version'] = function()
  local parser = HttpEnv.new("")
  parser:setHttpVersion("test http version")
  assert(parser:httpVersion() == 'test http version', parser:httpVersion())
end

return test
