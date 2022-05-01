local HttpEnv = require "arken.net.HttpEnv"
local HttpRequest = require "arken.net.HttpRequest"
local json = require "arken.json"
local mvm  = require "arken.mvm"

cache   = {}
cache.value = function()
end
cache.insert = function()
end

local test = {}

test.afterAll = function()
  _G.cache = nil
end

test.should_return_table_data = function()
  local env = {}
  env.field = function()
    return os.read(mvm.path() .. "/tests/lib/arken/net/cookie/1-example.cookie")
  end
  local request = HttpRequest.new{ _env = env }
  local session = request:session()
  assert( type(session) == 'table' )
end

test.should_return_table_restore_from_cache = function()
  local request = HttpRequest.new()
  cache.value = function()
    return json.encode({ myvalue = '123' })
  end
  local uuid    = os.uuid()
  local cookies = request.cookies
  request.cookies = function(val)
    return { arken_session_id = 'uuid-1234' }
  end
  request.__session_data = nil
  local session = request:session()

  assert( type(session) == 'table', type(session) )
  assert( session.myvalue == '123', session.myvalue )
end

test.should_return_table_data = function()
  local request = HttpRequest.new()
  request.cookies = function()
    return { arken_session_id = os.uuid() }
  end
  local session = request:session()
  assert( type(session) == 'table', type(session) )
  assert( #request._session_id == 36, #request._session_id )
end

test.should_return_empty_table_if_session_empty = function()
  local request = HttpRequest.new()
  cache.value = function()
    return ""
  end
  local uuid    = os.uuid()
  local cookies = request.cookies
  request.cookies = function(val)
    return { arken_session_id = 'uuid-1234' }
  end
  request._session_data = nil
  local session = request:session()

  assert( type(session) == 'table', type(session) )
  assert( json.encode(session) == '{}', json.encode(session) )
end

test.should_return_table_new_session = function()
  local request = HttpRequest.new()
  local uuid    = os.uuid()
  local cookies = request.cookies
  request.cookies = function(val)
    return { arken_session_id = nil }
  end
  local session = request:session()

  assert( type(session) == 'table', type(session) )
  assert( #request._session_id == 36, #request._session_id )
end

return test
