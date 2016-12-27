local helper = require('charon.coverage.helper')
local test   = {}

test.should_return_formated_number = function()
  local date = os.date("%Y/%m/%d %H:%M:%S")
  assert(helper.datetime() == date)
end

return test
