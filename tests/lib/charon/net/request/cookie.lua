request = require('charon.net.request')
local test = {}

test.before = function()
  request.reset()
end

test.should_append_header = function()
  request.cookie('box', 'full') 
  assert(request.__response[1] == 'Set-Cookie: box=full', request.__response[1])
end

test.should_append_header_with_http_only = function()
  request.cookie('box', 'full', { httponly = true }) 
  assert(request.__response[1] == 'Set-Cookie: box=full; HttpOnly', request.__response[1])
end

test.should_append_header_with_http_secure = function()
  request.cookie('box', 'full', { secure = true }) 
  assert(request.__response[1] == 'Set-Cookie: box=full; Secure', request.__response[1])
end

test.should_append_header_with_http_expires = function()
  request.cookie('box', 'full', { expires = '2017-01-13 15:30'}) 
  assert(request.__response[1] == 'Set-Cookie: box=full; Expires=2017-01-13 15:30', request.__response[1])
end

test.should_append_header_with_http_domain = function()
  request.cookie('box', 'full', { domain = 'charonplatform.org'}) 
  assert(request.__response[1] == 'Set-Cookie: box=full; Domain=charonplatform.org', request.__response[1])
end

test.should_append_header_with_http_path = function()
  request.cookie('box', 'full', { path = '/public'}) 
  assert(request.__response[1] == 'Set-Cookie: box=full; Path=/public', request.__response[1])
end

return test
