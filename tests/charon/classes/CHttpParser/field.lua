require "CHttpParser"

local test = {}

test['first example'] = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-1.header')
  local parser = CHttpParser.new(header)
  assert(parser:field("User-Agent") == "Mozilla/5.0 (X11; Linux x86_64; rv:38.0)")
end

return test