local json = require('charon.json')
local url  = require('charon.net.url')
local test = {}

test.should_return_table_form_url = function()
  local result = url.parse("http://username:password@hostname/path?arg=value#anchor")
  assert( result:build() == "http://username:password@hostname/path?arg=value#anchor", result:build() )
end

test.should_return_table_form_url_with_diferrent_port = function()
  local result = url.parse("http://username:password@hostname:2345/path?arg=value#anchor")
  assert( result:build() == "http://username:password@hostname:2345/path?arg=value#anchor", result:build() )
end

test.should_return_table_form_url_without_url = function()
  local result = url.parse("http://username:password@hostname:2345")
  assert( result:build() == "http://username:password@hostname:2345", result:build() )
end

return test
