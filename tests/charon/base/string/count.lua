local test = {}

test.should_return_1_in_ll_chars = function()
  local str = "hello world"
  assert( str:count("ll") == 1, str:count("ll") )
end

test.should_return_3_in_l_chars = function()
  local str = "hello world"
  assert( str:count("l") == 3, str:count("l") )
end

test.should_return_3_in_hello_chars = function()
  local str = "hello"
  assert( str:count("hello") == 1, str:count("hello") )
end

test.should_return_3_in_world_chars = function()
  local str = "world"
  assert( str:count("world") == 1, str:count("world") )
end

test.should_return_1_for_blank_space = function()
  local str = " "
  assert( str:count(" ") == 1, str:count(" ") )
end

test.should_return_0_unknow_char = function()
  local str = "hello world"
  assert( str:count("z") == 0, str:count("z") )
end

test.should_return_0_unknow_char = function()
  local str = "hello world"
  assert( str:count("z") == 0, str:count("z") )
end

test.should_return_1_special_chars = function()
  local str = "hello\r\nworld"
  assert( str:count("\r\n") == 1, str:count("\r\n") )
end

return test
