local HttpEnv = require "charon.net.HttpEnv"

local test   = {}

test['should return queryString id=1234&descricao=teste'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-1.txt')
  local parser = HttpEnv.new(header)
  assert(parser:queryString() == 'id=1234&descricao=teste')
end

test['should define query string'] = function()
  local parser = HttpEnv.new("")
  parser:setQueryString("id=999")
  assert(parser:queryString() == 'id=999', parser:queryString())
end

return test
