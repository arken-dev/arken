local test = {}

test.should_extension_file_name = function()
  local str = "teste.txt"
  local res = str:suffix(".")
  assert( res == 'txt', res )
end

test.should_extension_after_char = function()
  local str = "this is # a test"
  assert(str:suffix('#') == ' a test')
end

test['should return nil of not identify char'] = function()
  local str = "this is # a test"
  local res = str:suffix('*')
  assert(res == '', string.format("%s => type(%s)", res, type(res)))
end

test['should return complete suffix of many chars'] = function()
  local str = "this is a test"
  local res = str:suffix("is a")
  assert(res == " test", res)
end

return test
