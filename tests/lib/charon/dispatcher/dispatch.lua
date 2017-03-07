local json       = require('charon.json')
local dispatcher = require('charon.dispatcher')
local test = {}
package.path = package.path .. ';util/?.lua'

test.beforeAll = function()
  dispatcher.output = function() end
end

test.afterAll = function()
end

test.should_return_error_if_action_not_found = function()
  local env = {}

  env.requestPath = function()
    return "/order/unknow"
  end

  env.requestUri = function()
    return "/order/unknow"
  end

  dispatcher.prefix = ""
  local status, headers, body = dispatcher.dispatch(env)
  assert( status == 500, status )
  assert( #headers == 0, json.encode(headers) )
  assert( type(headers) == 'table', json.encode(headers) )
  assert( body:contains('action: "unknowAction" not found') == true, body )
end

test.should_return_error_if_action_not_found_in_production = function()
  local env = {}

  env.requestPath = function()
    return "/order/unknow"
  end

  env.requestUri = function()
    return "/order/unknow"
  end

  dispatcher.prefix = ""
  local status, headers, body = dispatcher.dispatch(env)
  assert( status == 500, status )
  assert( #headers == 0, json.encode(headers) )
  assert( type(headers) == 'table', json.encode(headers) )
  assert( body:contains('action: "unknowAction" not found') == true, body )
end


test.should_return_error_if_action_save = function()
  local env = {}
  env.requestPath = function()
    return "/order/save"
  end
  env.requestUri = function()
    return "/order/save"
  end

  dispatcher.prefix = ""
  local status, headers, body = dispatcher.dispatch(env)
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: text/html; charset=utf-8', headers[1] )
  assert( type(headers) == 'table', json.encode(headers) )
  assert( body == 'save !', body )
end

test.should_return_error_with_not_render = function()
  local env = {}
  env.requestPath = function()
    return "/order/test"
  end
  env.requestUri = function()
    return "/order/test"
  end

  dispatcher.prefix = ""
  CHARON_ENV='development'
  local status, message = pcall(dispatcher.dispatch, env)
  CHARON_ENV='test'
  assert( status == false, tostring(status) )
  assert( message:contains('body empty, missing render or return ?') == true, message )
end

test.should_return_public_image_in_development = function()
  local env = {}
  env.requestPath = function()
    return "/images/PIA00317.jpg"
  end
  env.requestUri = function()
    return "/images/PIA00317.jpg"
  end

  dispatcher.prefix = ""
  dispatcher.public = "util/public"
  CHARON_ENV='development'
  local status, headers, body = dispatcher.dispatch(env)
  CHARON_ENV='test'

  assert( status == 200, status )
  assert( headers[1] == 'Content-type: image/jpeg', headers[1] )
  assert( #body == 2059595, #body )
end

return test
