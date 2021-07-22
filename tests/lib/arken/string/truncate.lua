local test = {}

test.should_return_string_with_default_omission_e_separator = function()
  local str = "this is a test"
  local res = str:truncate(8)
  assert( res == "this...", res )
end

test.should_return_string_with_omission_with_begin_space = function()
  local str = "this is a test"
  local res = str:truncate(8, ' ...')
  assert( res == "this ...", res )
end

test.should_return_string_with_default_omission_e_without_separator = function()
  local str = "this is a test"
  local res = str:truncate(8, '...', '')
  assert( res == "this ...", res )
end

test.should_return_string_with_change_omission_e_without_separator = function()
  local str = "this is a test"
  local res = str:truncate(8, '###', '')
  assert( res == "this ###", res )
end

test.should_return_string_with_without_omission_e_without_separator = function()
  local str = "this is a test"
  local res = str:truncate(8, '', '')
  assert( res == "this is ", res )
end

test.should_return_string_with_without_omission_e_with_large_separator = function()
  local str = "this is a test"
  local res = str:truncate(13, '********', '')
  assert( res == "this ********", res )
end

test.should_return_string_if_omission_is_equal_size_string = function()
  local str = "this is a test"
  local res = str:truncate(4, '####', '')
  assert( res == "####", res )
end

test.should_return_large_string_with_defaults = function()
  local str = "this is a test, this is a test, this is a test, this is a test, this is a test"
  local res = str:truncate(65)
  assert( res == "this is a test, this is a test, this is a test, this is a...", res )
end

test.should_return_large_string_with_55_pos = function()
  local str = ("a"):rep(15000)
  local res = str:truncate(15000, '...', '')
  assert( res == (str:left(14997) .. '...'), res )
end

test.should_return_trucante_in_pos_if_separator_not_found = function()
  local str = ("this is a test !!!")
  local res = str:truncate(9, '...', '#')
  assert( res == 'this i...', res )
  local res = str:truncate(9, '...', ' ')
  assert( res == 'this...', res )
end

test.should_return_an_unchanged_string = function()
  local str = ("this is a test !!!")
  local res = str:truncate(250, '...', '#')
  assert( res == 'this is a test !!!', res )
end

return test
