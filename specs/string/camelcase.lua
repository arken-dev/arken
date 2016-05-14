local should = require "test.should"
local test   = {}

test['should upper case in underline'] = function()
  local str1 = "this_is_a_test"
  local str2 = "ThisIsATest"
  should.be_equal( str1:camelcase(), str2 )
end

test['should upper case in bar'] = function()
  local str1 = "active_record"
  local str2 = "ActiveRecord"
  should.be_equal( str1:camelcase(), str2 )
end

test['should upper case in underline and bar'] = function()
  local str1 = "active_record/adapter"
  local str2 = "ActiveRecord.Adapter"
  should.be_equal( str1:camelcase(), str2 )
end

test['should keep camelcase chars'] = function()
  local str1 = "classes/OByteArray"
  local str2 = "Classes.OByteArray"
  should.be_equal( str1:camelcase(), str2 )
end

test['should return empty string'] = function()
  local str1 = ""
  local str2 = ""
  should.be_equal( str1:camelcase(), str2 )
end

test['should return one string'] = function()
  local str1 = "c"
  local str2 = "C"
  should.be_equal( str1:camelcase(), str2 )
end

local examples = {
  html_api = 'HtmlApi',
  HTML_api = 'HTMLApi',
}

for source, destination in pairs(examples) do
  test['should convert ' .. source .. ' to ' .. destination] = function()
    should.be_equal(source:camelcase(), destination)
  end
end

return test
