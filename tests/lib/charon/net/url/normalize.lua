local json = require('charon.json')
local url  = require('charon.net.url')
local test = {}

test.should_return_table_form_url = function()
  local result = url.parse("http://username:password@hostname//path?form%5Bval1%5D=123&form%5Bval2%5D=strval")
  assert( result.path == "//path", result.path )
  assert( result:normalize().path == "/path", result:normalize().path )
end

test.should_return_table_form_url = function()
  string.normalize = url.normalize
  local url    = "HTTP://www.Example.com/"
  local result = url:normalize()
  string.normalize = nil
  assert( result:build() == "http://www.example.com/", json.encode(result:build()) )
end

test.should_return_table_form_url = function()
  string.normalize = url.normalize
  local url    = "HTTP://www.Example_Host.com/"
  local result = url:normalize()
  string.normalize = nil
  assert( result:build() == "http://www.example_host.com/", json.encode(result:build()) )
end

return test
