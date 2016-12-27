local json = require('charon.json')
local url  = require('charon.net.url')
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
end

return test
