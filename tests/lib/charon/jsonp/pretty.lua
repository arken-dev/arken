local json  = require 'charon.jsonp'
local tests = {}

tests.should_json_encode_string = function()
  local encoded = json.pretty("thisisatest")
  assert( json.decode(encoded) == "thisisatest" )
end

tests.should_json_encode_number = function()
  local encoded = json.pretty(1.234)
  assert( json.decode(encoded) == 1.234 )
end

tests.should_json_encode_number = function()
  local encoded = json.pretty(json.null)
  assert( encoded == "null", encoded )
end

tests.should_json_encode_table = function()
  local encoded = json.pretty{ id = 1234, description = "this is a test" }
  local value   = json.decode(encoded)
  assert(value.id == 1234)
  assert(value.description == "this is a test")
end

tests.should_json_encode_array = function()
  local encoded = json.pretty{1234, "this is a test"}
  local value   = json.decode(encoded)
  assert(value[1] == 1234)
  assert(value[2] == "this is a test")
end

tests.should_json_encode_table_inside_table = function()
  local encoded = json.pretty{ value = { id = 1234, description = "this is a test", flag = true } }
  local value   = json.decode(encoded).value
  assert(value.id == 1234)
  assert(value.description == "this is a test")
end

tests.should_json_encode_array_inside_table = function()
  local encoded = json.pretty{ value = { 1234, "this is a test", true } }
  local value   = json.decode(encoded).value
  assert(value[1] == 1234)
  assert(value[2] == "this is a test")
end

tests.should_json_encode_array_inside_array = function()
  local encoded = json.pretty{ { 1234, "this is a test" } }
  local value   = json.decode(encoded)[1]
  assert(value[1] == 1234)
  assert(value[2] == "this is a test")
end

return tests
