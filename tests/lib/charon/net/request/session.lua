local HttpParser = require "charon.net.HttpParser"
local json = require "charon.json"
request = require('charon.net.request')
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
  request.field = function(val)
    return os.read(CHARON_PATH .. "/tests/lib/charon/net/cookie/1-example.cookie")
  end
  local session = request.session()
  assert( type(session) == 'table' )
end

test.should_return_table_restore_from_cache = function()
  cache.value = function()
    return json.encode({ myvalue = '123' })
  end    
  local uuid    = os.uuid()
  local cookies = request.cookies
  request.cookies = function(val)
    return { charon_session_id = 'uuid-1234' }
  end
  request.__session_data = nil
  local session = request.session()
  request.cookies = cookies
  assert( type(session) == 'table', type(session) )
  assert( session.myvalue == '123', session.myvalue )
end


test.should_return_table_data = function()
  request.reset() 
  local uuid    = os.uuid()
  local cookies = request.cookies
  request.cookies = function(val)
    return { charon_session_id = uuid }
  end
  local session = request.session()
  request.cookies = cookies
  assert( type(session) == 'table', type(session) )
  assert( #request.__session_id == 36, #request.__session_id )
end

test.should_return_empty_table_if_session_empty = function()
  cache.value = function()
    return ""
  end    
  local uuid    = os.uuid()
  local cookies = request.cookies
  request.cookies = function(val)
    return { charon_session_id = 'uuid-1234' }
  end
  request.__session_data = nil
  local session = request.session()
  request.cookies = cookies
  assert( type(session) == 'table', type(session) )
  assert( json.encode(session) == '{}', json.encode(session) )
end


test.should_return_table_new_session = function()
  
  local uuid    = os.uuid()
  local cookies = request.cookies
  request.cookies = function(val)
    return { charon_session_id = nil }
  end
  local session = request.session()
  request.cookies = cookies
  assert( type(session) == 'table', type(session) )
  assert( #request.__session_id == 36, #request.__session_id )
end

return test
