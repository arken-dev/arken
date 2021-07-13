local test = {}

test["should remove \r\n"] = function()
  local str = "string\r\n"
  local res = str:chop()
  assert( res == "string", res )
end

test["should remove last \r"] = function()
  local str = "string\n\r"
  local res = str:chop()
  assert( res == "string\n", res )
end

test["should remove last \n"] = function()
  local str = "string\n"
  local res = str:chop()
  assert( res == "string", res )
end

test["should remove last char"] = function()
  local str = "string"
  local res = str:chop()
  assert( res == "strin", res )
end

test["should remove blank se one char"] = function()
  local str = "x"
  local res = str:chop()
  assert( res == "", res )
end

test["should empty string if blank"] = function()
  local str = ""
  local res = str:chop()
  assert( res == "", res )
end

return test
