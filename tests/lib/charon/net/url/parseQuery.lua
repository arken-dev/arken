local json = require('charon.json')
local url  = require('charon.net.url')
local test = {}

test.should_return_table_of_parse = function()
  local result = url.parseQuery("val1=123&val2=strval")
  assert(type(result) == 'table')
  assert(result.val1 == '123', json.encode(result))
  assert(result.val2 == 'strval', json.encode(result))
end

test.should_return_table_if_nil = function()
  local result = url.parseQuery(nil)
  assert(type(result) == 'table')
end

test.should_return_table_if_empty_string = function()
  local result = url.parseQuery('')
  assert(type(result) == 'table')
end

test.should_return_multiple_table = function()
  local result = url.parseQuery("form[val1]=123&form[val2]=strval")
  assert(type(result) == 'table')
  assert(type(result.form) == 'table')
  assert(result.form.val1 == '123', json.encode(result))
  assert(result.form.val2 == 'strval', json.encode(result))
end

test.should_return_converting_param_with_array = function()
  local result = url.parseQuery("val%5B1%5D=val1&val%5B2%5D=val2&val%5B3%5D=val3")
  assert(type(result.val) == "table", json.encode(result))
  assert(result.val[1] == "val1", result.val[1])
end

return test
