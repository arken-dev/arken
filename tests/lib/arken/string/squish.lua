local test   = {}

test['should remove blanks from the beginning and the end'] = function()
  local str1 = "  this is a test  "
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should remove blanks from the left'] = function()
  local str1 = "  this is a test"
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should remove blanks from the right'] = function()
  local str1 = "this is a test     "
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end


test['should remove one char blank from the beginning and the end'] = function()
  local str1 = " this is a test "
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should remove blanks throughout the string'] = function()
  local str1 = "   this   is   a   test   "
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should remove break line throughout the string'] = function()
  local str1 = "\nthis\nis\na\ntest\n"
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should remove new lines and white spaces'] = function()
  local str1 = "\n \nthis is a test\n \n"
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should remove tab throughout the string'] = function()
  local str1 = "\tthis\tis\ta\ttest\t"
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should remove blank, break line and tab throughout the string'] = function()
  local str1 = " \n\t this \t\n is \t\n a \t\n test \t\n "
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
  local str1 = "\n \tthis\t \nis\t \na\t \ntest\t \n"
  local str2 = "this is a test"
  assert( str1:squish() == str2 )
end

test['should return empty string for many invalid chars'] = function()
  local str = "   "
  assert( str:squish() == "" )

  str = "\n\r "
  assert( str:squish() == "" )

  str = "\n"
  assert( str:squish() == "" )

  str = " "
  assert( str:squish() == "" )
end

test['should remove special char with utf-8 chars'] = function()
  local str1 = " \n\tnão\n\t "
  local str2 = "não"
  assert( str1:squish() == str2 )
end

test['should equal when not special chars'] = function()
  local str = "cadastro de Contato_Tipo"
  assert( str:squish() == str )
end

test['should equal one char'] = function()
  local str = "a"
  assert( str:squish() == str )
end

test['should equal empty string'] = function()
  local str = ""
  assert( str:squish() == str )
end

test['should string with one char'] = function()
  local str1 = "("
  local str2 = "("
  assert( str1:squish() == str2, str1:squish() )
end

test['should string with one char with space right'] = function()
  local str1 = "( "
  local str2 = "("
  assert( str1:squish() == str2, str1:squish() )
end

test['should string with one char with space left'] = function()
  local str1 = " ("
  local str2 = "("
  assert( str1:squish() == str2, str2:squish() )
end

test['should string with one char with space right'] = function()
  local str1 = "( "
  local str2 = "("
  assert( str1:squish() == str2, str1:squish() )
end

return test
