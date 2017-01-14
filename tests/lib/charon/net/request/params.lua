local HttpParser = require "charon.net.HttpParser"
request = require('charon.net.request')
local test = {}

test.should_return_table_from_params_by_get = function()
  request.requestMethod = function()
    return "GET"
  end
  request.queryString = function()
    return "val1=123&val2=strval"
  end
  request.field = function(val)
    return ''
  end
  local params = request.params(true)
  assert( type(params) == 'table', type(params) )
  assert(params.val1 == '123', params.val1)
  assert(params.val2 == 'strval', params.val2)
end

test.should_return_table_from_params_by_post = function()
  request.requestMethod = function()
    return "POST"
  end
  request.headerDone= function()
    return "val1=444&val2=555"
  end
  local params = request.params(true)
  assert(params.val1 == '444', params.val1)
  assert(params.val2 == '555', params.val2)
end

test.should_return_table_from_params_by_multipart = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/net/HttpParser/example-2.header')
  local parser = HttpParser.new(header)

  request.requestMethod = function()
    return "POST"
  end
  request.headerDone = function()
    return parser:headerDone()
  end
  request.field = function(val)
    return 'multipart/form-data;'
  end

  local params = request.params(true)
  assert(params.id == '12.341.234', params.id)
end

return test
