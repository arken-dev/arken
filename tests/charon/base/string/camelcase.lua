local test = {}

test['should upper case in underline'] = function()
  local str1 = "this_is_a_test"
  local str2 = "ThisIsATest"
  assert( str1:camelcase() == str2 )
end

test['should upper case in bar'] = function()
  local str1 = "active_record"
  local str2 = "ActiveRecord"
  assert( str1:camelcase() == str2 )
end

test['should upper case in underline and bar'] = function()
  local str1 = "active_record/adapter"
  local str2 = "ActiveRecord.Adapter"
  assert( str1:camelcase() == str2 )
end

test['should keep camelcase chars'] = function()
  local str1 = "classes/CByteArray"
  local str2 = "Classes.CByteArray"
  assert( str1:camelcase() == str2 )
end

test['should return empty string'] = function()
  local str1 = ""
  local str2 = ""
  assert( str1:camelcase() == str2 )
end

test['should return one string'] = function()
  local str1 = "c"
  local str2 = "C"
  assert( str1:camelcase() == str2 )
end

local examples = {
  html_api = 'HtmlApi',
  HTML_api = 'HTMLApi',
}

for source, destination in pairs(examples) do
  test['should convert ' .. source .. ' to ' .. destination] = function()
    assert(source:camelcase() == destination)
  end
end

return test
