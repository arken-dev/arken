require "CHttpParser"

local test = {}

test['first example'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-1.header')
  local parser = CHttpParser.new(header)
  assert(parser:fragment() == nil)
end

return test
