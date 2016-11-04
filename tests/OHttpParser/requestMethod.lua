require "OHttpParser"

local test = {}

test['deve retornar GET'] = function()
  local header = os.read(OBERON_PATH .. '/specs/OHttpParser/example-1.header')
  local parser = OHttpParser.new(header)
  assert(parser:requestMethod() == 'GET')
end

return test
