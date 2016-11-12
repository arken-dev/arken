require "OHttpParser"

local test = {}

test['first example'] = function()
  local header = os.read(OBERON_PATH .. '/tests/OHttpParser/example-1.header')
  local parser = OHttpParser.new(header)
  assert(parser:fragment() == nil)
end

return test
