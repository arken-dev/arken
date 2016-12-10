local test = {}

test.should_return_new_date_add_days = function()
  local date1 = QDateTime.fromString("1975/03/21 12:20:30", "yyyy/MM/dd hh:mm:ss")
  local date2 = date1:addDays(5)
  assert(date2:toString() == '1975/03/26 12:20:30.000', date2:toString())
end 

return test
