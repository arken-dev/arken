local Converter  = require 'charon.Converter'
local HttpClient = require 'charon.net.HttpClient'
local test       = {}

test.should_return_userdata = function()
  local client = HttpClient.new('http://www.google.com.br')
  local c = Converter.new{ value = client }
  assert( c:inspect('value') == 'userdata' )
end

test.should_return_json = function()
  local c = Converter.new{ value = { id = 1, name = 'John' } }
  local result = c:inspect('value')
  assert( result  == [[{"name":"John","id":1}]], result )
end

test.should_return_self_json = function()
  local c = Converter.new{ value = { id = 1, name = 'John' } }
  local result = c:inspect()
  assert( result  == [[{"value":{"name":"John","id":1}}]], result )
end


return test
