local HttpEnv = require "charon.net.HttpEnv"
local HttpRequest = require "charon.net.HttpRequest"
local test = {}

test.should_return_table_from_cookies = function()
  local env = {}
  env.field = function(val)
    return os.read(CHARON_PATH .. "/tests/lib/charon/net/cookie/1-example.cookie")
  end
  local request = HttpRequest.new{ _env = env }
  local result = request:cookies()
  assert(result.__gads == "ID=65db397af496b42f:T=1459784587:S=ALNI_MaXlE55QEou3h9txkA8jA5kPzMKVg")
end

return test
