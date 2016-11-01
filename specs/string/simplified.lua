local test   = {}

test['should remove blanks from the beginning and the end'] = function()
  local str1 = "  this is a test  "
  local str2 = "this is a test"
  assert( str1:simplified() == str2 )
end

test['should remove one char blank from the beginning and the end'] = function()
  local str1 = " this is a test "
  local str2 = "this is a test"
  assert( str1:simplified() == str2 )
end

test['should remove blanks throughout the string'] = function()
  local str1 = "   this   is   a   test   "
  local str2 = "this is a test"
  assert( str1:simplified() == str2 )
end

test['should remove break line throughout the string'] = function()
  local str1 = "\nthis\nis\na\ntest\n"
  local str2 = "this is a test"
  assert( str1:simplified() == str2 )
end

test['should remove tab throughout the string'] = function()
  local str1 = "\tthis\tis\ta\ttest\t"
  local str2 = "this is a test"
  assert( str1:simplified() == str2 )
end

test['should remove blank, break line and tab throughout the string'] = function()
  local str1 = " \n\t this \t\n is \t\n a \t\n test \t\n "
  local str2 = "this is a test"
  assert( str1:simplified() == str2 )
  local str1 = "\n \tthis\t \nis\t \na\t \ntest\t \n"
  local str2 = "this is a test"
  assert( str1:simplified() == str2 )
end

test['should return empty string for many invalid chars'] = function()
  local str = "   "
  assert( str:simplified() == "" )

  str = "\n\r "
  assert( str:simplified(), == "" )

  str = " "
  assert( str:simplified(), == "" )
end

test['should remove special char with utf-8 chars'] = function()
  local str1 = " \n\tnão\n\t "
  local str2 = "não"
  assert( str1:simplified() == str2 )
end

test['should equal when not special chars'] = function()
  local str = "cadastro de Contato_Tipo"
  assert( str:simplified(), == str )
end

test['should equal one char'] = function()
  local str = "a"
  assert( str:simplified(), == str )
end

test['should equal empty string'] = function()
  local str = ""
  assert( str:simplified(), == str )
end

return test
