local Date = require('arken.chrono.Date')
local test = {}

test.should_parse_date_only = function()
  local str = "2015/07/04"
  local dt  = Date.parse(str)
  assert( dt:toString() == '2015-07-04', dt:toString() )
end

test.should_parse_date_with_trace = function()
  local str = "2015-07-04"
  local dt  = Date.parse(str)
  assert( dt:toString() == '2015-07-04', dt:toString() )
end

test.should_parse_date_br = function()
  local str = "04/07/2015"
  local dt  = Date.parse(str)
  assert( dt:toString() == '2015-07-04', dt:toString() )
end

test.should_parse_date_with_trace = function()
  local str = "04-07-2015"
  local dt  = Date.parse(str)
  assert( dt:toString() == '2015-07-04', dt:toString() )
end

test.should_return_nil_for_invalid_date2 = function()
  local str = "2015/07/32"
  local dt  = Date.parse(str)
  assert( dt == nil, type(dt) )
end

return test
