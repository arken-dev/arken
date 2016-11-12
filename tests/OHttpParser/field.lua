require "OHttpParser"

local test = {}

test['first example'] = function()
  local header = os.read(OBERON_PATH .. '/tests/OHttpParser/example-1.header')
  local parser = OHttpParser.new(header)
  assert(parser:field("User-Agent") == "Mozilla/5.0 (X11; Linux x86_64; rv:38.0)")
end

return test
