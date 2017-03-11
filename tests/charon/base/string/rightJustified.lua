local test = {}

test.should_return_self_string_if_size_if_minor_of_the_self = function()
  local str = "my str"
  local result = str:rightJustified(2, "")
  assert( result == "my str", result )
end

test.should_return_centered_with_sharp = function()
  local str    = "my str"
  local result = str:rightJustified(10, "#")
  assert( result == "####my str", result )
end

test.should_return_centered_with_space = function()
  local str    = "my str"
  local result = str:rightJustified(10, " ")
  assert( result == "    my str", result )
end

test.should_return_centered_with_sharp_odd_number = function()
  local str    = "my str"
  local result = str:rightJustified(9, "#")
  assert( result == "###my str", result )
end

test.should_return_self_string_if_empty_pad = function()
  local str    = "my str"
  local result = str:rightJustified(9, "")
  assert( result == "my str", result )
end

test.should_return_string_width_odd_pad = function()
  local str    = "my str"
  local result = str:rightJustified(9, "-#-")
  assert( result == "-#-my str", result )
end

test.should_return_string_width_size_is_big_major_padding = function()
  local str    = "my str"
  local result = str:rightJustified(26, "-#-")
  assert( result == "-#--#--#--#--#--#--#my str", result )
end

return test
