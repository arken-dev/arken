local test = {}

test.should_return_dasherize_with_multiple_upper_case = function()
  local str = "KING free style MMA"
  local res = str:dasherize()
  assert( res == "king-free-style-mma", res )
end

test.should_return_word_capitalize = function()
  local str = "Order"
  local res = str:dasherize()
  assert( res == "order", res )
end

test.should_return_words_with_capitalize = function()
  local str = "OrderItem"
  local res = str:dasherize()
  assert( res == "order-item", res )
end

test.should_return_words_with_capitalize_with_dasherize = function()
  local str = "Order_Item"
  local res = str:dasherize()
  assert( res == "order-item", res )
end

test.should_return_with_begin_space_and_capitalize = function()
  local str = " Order"
  local res = str:dasherize()
  assert( res == "order", res )
end

test.should_return_with_begin_space_and_lowercase = function()
  local str = " order"
  local res = str:dasherize()
  assert( res == "order", res )
end

test.should_return_with_begin_and_after_space = function()
  local str = "   order   "
  local res = str:dasherize()
  assert( res == "order", res )
end

test.should_return_with_begin_and_after_and_middle_space = function()
  local str = "   order   item     "
  local res = str:dasherize()
  assert( res == "order-item", res )
end

return test
