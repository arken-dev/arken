local ctest = require 'arken.test'
local test  = {}

test.should_error_if_file_not_exists = function()
  ctest.output = function() end
  local status, message = pcall(ctest.execute, {"file-not-exists.lua"})
  assert( status == false )
  assert( message:contains("file-not-exists.lua not exists"), message )
  ctest.output = io.write
end

return test
