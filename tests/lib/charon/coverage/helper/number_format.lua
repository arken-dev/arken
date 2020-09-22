local helper = require('arken.coverage.helper')
local test   = {}

test.should_return_formated_number = function()
  assert(helper.number_format(1234.35) == '1234.3500', helper.number_format(1234.35))
end

return test
