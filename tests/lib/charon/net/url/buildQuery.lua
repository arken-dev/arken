local json = require('charon.json')
local url  = require('charon.net.url')
local test = {}

test.should_return_table_of_parse = function()
  local result = url.buildQuery({val1 = "123", val2 = "strval"})
  assert(type(result) == 'string')
  assert(result == "val1=123&val2=strval")
end

test.should_errors_if_nil = function()
  local result, message = pcall(url.buildQuery, nil)
  assert(result == false)
  assert(message:contains("(table expected, got nil)") == true )
end

test.should_errors_if_string = function()
  local result, message = pcall(url.buildQuery, '')
  assert(result == false)
  assert(message:contains("(table expected, got string)") == true )
end

test.should_return_multiple_table = function()
  local result = url.buildQuery({form = {val1 = '123', val2 = 'strval'}})
  assert(result == "form%5Bval1%5D=123&form%5Bval2%5D=strval", result)
end

test.should_return_converting_encoding_url = function()
  local result = url.buildQuery({value = "my%20space"})
  assert(result == "value=my%2520space", result)
end

test.should_return_converting_encoding_url = function()
  local result = url.buildQuery({val = {"val1", "val2", "val3"}})
  assert(result == "val%5B1%5D=val1&val%5B2%5D=val2&val%5B3%5D=val3", json.encode(result))
end

test.should_return_query_with_blank_string = function()
  local result = url.buildQuery({val = {"val1", "", "val3"}})
  assert(result == "val%5B1%5D=val1&val%5B2%5D&val%5B3%5D=val3", json.encode(result))
end

return test
