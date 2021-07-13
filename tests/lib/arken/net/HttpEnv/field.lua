local HttpEnv = require "arken.net.HttpEnv"

local test = {}

test['should return User-Agent field'] = function()
  local header = os.read(ARKEN_PATH .. '/tests/arken/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:field("User-Agent") == "Mozilla/5.0 (X11; Linux x86_64; rv:38.0)")
end

test['should set field'] = function()
  local parser = HttpEnv.new("")
  parser:setField("User-Agent", "Chrome")
  assert(parser:field("User-Agent") == "Chrome", parser:field("User-Agent"))
end

return test
