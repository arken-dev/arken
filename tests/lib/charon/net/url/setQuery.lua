local json = require('arken.json')
local url  = require('arken.net.url')
local test = {}

test.should_return_table_form_string_arg = function()
  local result = url.parse("http://username:password@hostname/path?arg=value#anchor")
  assert( result:build() == "http://username:password@hostname/path?arg=value#anchor", result:build() )
  result:setQuery("arg=other")
  assert( result:build() == "http://username:password@hostname/path?arg=other#anchor", result:build() )
end

test.should_return_table_form_table_arg = function()
  local result = url.parse("http://username:password@hostname/path?arg=value#anchor")
  assert( result:build() == "http://username:password@hostname/path?arg=value#anchor", result:build() )
  result:setQuery({arg = "other"})
  assert( result:build() == "http://username:password@hostname/path?arg=other#anchor", result:build() )
end

return test
