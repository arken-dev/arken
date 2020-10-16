local test = {}

test.should_extension_file_name = function()
  local str = "teste.txt"
  assert(str:suffix() == 'txt')
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

return test
