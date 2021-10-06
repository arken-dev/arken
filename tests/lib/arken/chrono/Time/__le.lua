local Time = require('arken.chrono.Time')
local test = {}

test['should return true for equals dates'] = function()
  local d1 = Time.parse('2021-03-21')
  local d2 = Time.parse('2021-03-21')
  assert( ( d1 <= d2 ) == true )
  assert( ( d2 <= d1 ) == true )
end

test['should return true for equals dates with hours'] = function()
  local d1 = Time.parse('2021-03-21 12:30:05')
  local d2 = Time.parse('2021-03-21 12:30:05')
  assert( ( d1 <= d2 ) == true )
  assert( ( d2 <= d1 ) == true )
end

test['should return false for dt1 minor'] = function()
  local d1 = Time.parse('2021-03-20')
  local d2 = Time.parse('2021-03-21')
  assert( ( d1 <= d2 ) == true )
  assert( ( d2 <= d1 ) == false )
end

test['should return false for dt1 minor with hours'] = function()
  local d1 = Time.parse('2021-03-20 12:30:05')
  local d2 = Time.parse('2021-03-21 12:30:05')
  assert( ( d1 <= d2 ) == true )
  assert( ( d2 <= d1 ) == false )
end

return test
