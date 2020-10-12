local helper = require('arken.coverage.helper')
local test   = {}

test.should_return_without_inicial_char = function()
  local fileName = "@/usr/local"
  assert( helper.sanitize(fileName) == "/usr/local", helper.sanitize(fileName) )
end

test.should_return_without_inicial_char_point_local_file = function()
  local fileName = "./usr/local"
  assert( helper.sanitize(fileName) == "usr/local", helper.sanitize(fileName) )
end

return test
