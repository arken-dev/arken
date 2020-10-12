local HttpRequest = require('arken.net.HttpRequest')
local test = {}

test.before = function()
end

test.should_append_header = function()
  local request = HttpRequest.new()
  request:cookie('box', 'full')
  assert(request._response[1] == 'Set-Cookie: box=full', request._response[1])
end

test.should_append_header_with_http_only = function()
  local request = HttpRequest.new()
  request:cookie('box', 'full', { httponly = true })
  assert(request._response[1] == 'Set-Cookie: box=full; HttpOnly', request._response[1])
end

test.should_append_header_with_http_secure = function()
  local request = HttpRequest.new()
  request:cookie('box', 'full', { secure = true })
  assert(request._response[1] == 'Set-Cookie: box=full; Secure', request._response[1])
end

test.should_append_header_with_http_expires = function()
  local request = HttpRequest.new()
  request:cookie('box', 'full', { expires = '2017-01-13 15:30'})
  assert(request._response[1] == 'Set-Cookie: box=full; Expires=2017-01-13 15:30', request._response[1])
end

test.should_append_header_with_http_domain = function()
  local request = HttpRequest.new()
  request:cookie('box', 'full', { domain = 'arkenplatform.org'})
  assert(request._response[1] == 'Set-Cookie: box=full; Domain=arkenplatform.org', request._response[1])
end

test.should_append_header_with_http_path = function()
  local request = HttpRequest.new()
  request:cookie('box', 'full', { path = '/public' })
  assert(request._response[1] == 'Set-Cookie: box=full; Path=/public', request._response[1])
end

return test
