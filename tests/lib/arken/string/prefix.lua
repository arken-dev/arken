local test = {}

test['should name is filename without extension'] = function()
  local str = "teste.txt"
  local res = str:prefix(".")
  assert( res == 'teste', res )
end

test['should prefix before char'] = function()
  local str = "this is # a test"
  local res = str:prefix('#')
  assert(res == 'this is ', res)
end

test['should return complete name of not identify char'] = function()
  local str = "this is # a test"
  local res = str:prefix('*')
  assert(res == "", string.format("%s => type(%s)", res, type(res)))
end

test['should return complete prefix of many chars'] = function()
  local str = "this is a test"
  local res = str:prefix("is a")
  assert(res == "this ", res)
end

test['should return complete prefix of many chars'] = function()
  local str = "this is a test"
  local res = str:prefix("is a")
  assert(res == "this ", res)
end

return test
