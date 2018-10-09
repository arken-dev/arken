local json = require('charon.json')
local url  = require('charon.net.url')
local test = {}

test.should_return_table_of_parse = function()
  local result = url.parseQuery("val1=123&val2=strval")
  assert(type(result) == 'table')
  assert(result.val1 == '123', json.encode(result))
  assert(result.val2 == 'strval', json.encode(result))
end

test.should_return_value_of_query_with_data_format = function()
  local result = url.parseQuery("date1=01%2F10%2F2018&date2=31%2F10%2F2018&x=36&y=4")
  assert(type(result) == 'table')
  assert(result.date1 == '01/10/2018', result.date1)
  assert(result.date2 == '31/10/2018', result.date2)
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

test.should_return_array_query = function()
  local result = url.parseQuery("var[]=1234&var[]=2345&var[]=9876")
  assert(result.var[1] == '1234', result.var[1] )
  assert(result.var[2] == '2345', result.var[2] )
  assert(result.var[3] == '9876', result.var[3] )
end

test.should_return_hash_and_hash_query = function()
  local result = url.parseQuery("var[1234][id]=1234&var[1234][name]=John")
  assert(result.var[1234].id == '1234')
  assert(result.var[1234].name == 'John')
end

test.should_return_hash_and_hash_query_and_empty_var = function()
  local result = url.parseQuery("var[1234][id]=1234&var[1234][name]=John&var[2345][id]=&var[2345][name]=")
  assert(result.var[2345].id == '')
  assert(result.var[2345].name == '')
end

test.should_return_hash_and_array_query = function()
  local result = url.parseQuery("var[1234][]=1234&var[1234][]=John")
  assert(result.var[1234][1] == '1234')
  assert(result.var[1234][2] == 'John')
end

test.should_return_array_inside_array = function()
  local result = url.parseQuery("var[][]=1234&var[][]=John")
  assert(result.var[1][1] == '1234')
  assert(result.var[2][1] == 'John')
end

return test
