local helper = require 'arken.Helper'
local test   = {}

test.should_return_timestamp_file_system = function()
  helper.publicPath= 'util'
  local timestamp = helper:fileTimestamp("/images/black.png")
  assert( timestamp == os.ctime("util/images/black.png"), timestamp )
end

test.should_return_timestamp_file_system_not_exists = function()
  helper.publicPath= 'util'
  local timestamp = helper:fileTimestamp("/images/notexists.png")
  assert( timestamp == 0 )
end

return test
