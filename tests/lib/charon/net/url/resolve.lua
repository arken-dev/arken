local json = require('charon.json')
local url  = require('charon.net.url')
local test = {}

test.should_return_table_form_url = function()
  local url1   = "http://username:password@hostname/path?arg=value#anchor"
  local url2   = "http://username:password@hostname/path?arg=1234"
  local result = url.parse(url1)
  assert( result:resolve("?arg=1234"):build() == url2, result:resolve("?arg=1234"):build() )
end

test.should_return_table_change_params = function()
  local result = url.parse("http://username:password@hostname:2345/path?arg=value#anchor")
  assert( result:build() == "http://username:password@hostname:2345/path?arg=value#anchor", result:build() )
end

test.should_return_table_change_params = function()
  string.resolve = url.resolve
  local url1    = "http://username:password@hostname/path?arg=value#anchor"
  local url2    = "http://username:password@hostname/path?arg=1234"
  local result  = url1:resolve("?arg=1234")
  assert( result:build() == url2, result:build() )
end

return test
