local DateTime = require('charon.time.DateTime')
local test = {}

test.should_parse_date_only = function()
  local str = "2015-07-04"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 00:00:00.000', dt:toString() )
end

test.should_parse_date_with_trace = function()
  local str = "2015-07-04"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 00:00:00.000', dt:toString() )
end

test.should_parse_date_br = function()
  local str = "04/07/2015"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 00:00:00.000', dt:toString() )
end

test.should_parse_date_with_trace = function()
  local str = "04-07-2015"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 00:00:00.000', dt:toString() )
end

test.should_parse_date_hour_minute = function()
  local str = "2015/07/04 13:45"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 13:45:00.000', dt:toString() )
end

test.should_parse_date_hour_minute_second = function()
  local str = "2015/07/04 13:45:30"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 13:45:30.000', dt:toString() )
end

test.should_parse_date_with_1msec = function()
  local str = "2015/07/04 13:45:30.1"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 13:45:30.100', dt:toString() )
end

test.should_parse_date_with_2msec = function()
  local str = "2015/07/04 13:45:30.12"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 13:45:30.120', dt:toString() )
end

test.should_parse_date_with_3msec = function()
  local str = "2015/07/04 13:45:30.125"
  local dt  = DateTime.parse(str)
  assert( dt:toString() == '2015-07-04 13:45:30.125', dt:toString() )
end

test.should_return_nil_for_invalid_date1 = function()
  local str = "2015/07/04 abc"
  local dt  = DateTime.parse(str)
  assert( dt == nil, type(dt) )
end

test.should_return_nil_for_invalid_date2 = function()
  local str = "2015/07/32"
  local dt  = DateTime.parse(str)
  assert( dt == nil, type(dt) )
end

test.should_return_nil_for_invalid_date3 = function()
  local str = "2015/07/32 99:45"
  local dt  = DateTime.parse(str)
  assert( dt == nil, type(dt) )
end

return test
