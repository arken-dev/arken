local QDateTime = require "QDateTime"

local test = {}

test.should_concat_string_and_userdata = function()
  local str = "data :"
  local dt  = QDateTime.currentDateTime()
  local result = str .. dt
  assert( result == str .. dt:toString("yyyy/MM/dd hh:mm:ss") )
end

test.should_concat_userdata_and_string = function()
  local dt  = QDateTime.currentDateTime()
  local str = "data :"
  local result = dt .. str
  assert( result == dt:toString("yyyy/MM/dd hh:mm:ss") .. str )
end

return test
