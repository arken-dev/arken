local json = require('arken.json')
local url  = require('arken.net.url')
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

test.should_return_encoded_path = function()
  local result = url.parse("http://newlink.org/user/user=riveira")
  assert( result:build() == "http://newlink.org/user/user=riveira", result:build() )
end

test.should_return_major_char_path = function()
  local result = url.parse("http://newlink.org/user/user>riveira")
  assert( result:build() == "http://newlink.org/user/user>riveira", result:build() )
end


return test
