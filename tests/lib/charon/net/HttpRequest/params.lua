local HttpEnv     = require "charon.net.HttpEnv"
local HttpRequest = require "charon.net.HttpRequest"
local test = {}

test.should_return_table_from_params_by_get = function()
  local env = {}
  env.requestMethod = function()
    return "GET"
  end
  env.queryString = function()
    return "val1=123&val2=strval"
  end
  env.field = function(val)
    return ''
  end
  local request = HttpRequest.new{ _env = env }
  local params = request:params(true)
  assert( type(params) == 'table', type(params) )
  assert(params.val1 == '123', params.val1)
  assert(params.val2 == 'strval', params.val2)
end

test.should_return_table_from_params_by_post = function()
  local env = {}
  env.requestMethod = function()
    return "POST"
  end
  env.headerDone= function()
    return "val1=444&val2=555"
  end
  env.field = function(val)
    return ''
  end
  local request = HttpRequest.new{ _env = env }
  local params  = request:params(true)
  assert(params.val1 == '444', params.val1)
  assert(params.val2 == '555', params.val2)
end

test.should_return_table_from_params_by_multipart = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpEnv/example-2.txt')
  local env = HttpEnv.new(header)
  local request = HttpRequest.new{ _env = env }
  local params = request:params(true)
  assert(params.id == '12.341.234', params.id)
end

return test
