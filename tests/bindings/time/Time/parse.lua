local Time = require('arken.time.Time')
local test = {}

test.should_parse_hour_and_minute = function()
  local str = "13:30"
  local dt  = Time.parse(str)
  assert( dt:toString() == '13:30:00.0', dt:toString() )
end

test.should_parse_hour_and_minute_and_second = function()
  local str = "13:30:04"
  local dt  = Time.parse(str)
  assert( dt:toString() == '13:30:04.0', dt:toString() )
end

test.should_parse_with_msecs1 = function()
  local str = "13:30:04.1"
  local dt  = Time.parse(str)
  assert( dt:toString('hh:mm:ss.z') == '13:30:04.100', dt:toString('hh:mm:ss.z') )
end

test.should_parse_with_msecs2 = function()
  local str = "13:30:04.15"
  local dt  = Time.parse(str)
  assert( dt:toString('hh:mm:ss.z') == '13:30:04.150', dt:toString('hh:mm:ss.z') )
end

test.should_parse_with_msecs3 = function()
  local str = "13:30:04.157"
  local dt  = Time.parse(str)
  assert( dt:toString('hh:mm:ss.z') == '13:30:04.157', dt:toString('hh:mm:ss.z') )
end

test.should_return_nil_for_invalid_string = function()
  local str = "13"
  local dt  = Time.parse(str)
  assert( dt == nil, type(dt) )
end

return test
