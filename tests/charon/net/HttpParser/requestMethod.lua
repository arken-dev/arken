require "CHttpParser"

local test = {}

test['deve retornar GET'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-1.header')
  local parser = CHttpParser.new(header)
  assert(parser:requestMethod() == 'GET')
end

return test
