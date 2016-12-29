local test = {}

test.should_return_string_with_default_omission_e_separator = function()
  local str = "this is a test"
  local res = str:truncate(8)
  assert( res == "this...", res )
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
  local res = str:truncate(15, '********', '')
  assert( res == "this is********", res )
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
--[[
test.should_return_large_string_with_55_pos = function()
  local str = "this is a test, this is a test, this is a test, this is a test, this is a test"
  local res = str:truncate(59)
  assert( res == "this is a test, this is a test, this is a test, this is a...", res )
end
]]
return test
