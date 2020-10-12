local json = require('arken.json')
local url  = require('arken.net.url')
local test = {}

test.should_return_table_form_url = function()
  local result = url.parse("http://username:password@hostname/path?arg=value#anchor")
  assert(type(result)       == 'table', json.encode(result))
  assert(result.user        == 'username', json.encode(result))
  assert(result.fragment    == 'anchor', json.encode(result))
  assert(result.host        == 'hostname', json.encode(result))
  assert(result.scheme      == 'http', json.encode(result))
  assert(type(result.query) == 'table', json.encode(result))
  assert(result.query.arg   == 'value', json.encode(result))
  assert(result.userinfo    == 'username:password', json.encode(result))
  assert(result.authority   == 'username:password@hostname', json.encode(result))
  assert(result.password    == 'password', json.encode(result))
  assert(result.path        == '/path', result.path)
end

test.should_ipv6_table = function()
  local result = url.parse("http://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]:80/index.html")
   assert(result.host  == '[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]', json.encode(result))
end

return test
