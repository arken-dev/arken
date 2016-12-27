local helper = require('charon.coverage.helper')
local test   = {}

test.shoul_change_blank_space_for_nbsp = function()
  local var = " this is a test "
  assert( helper.spacing(var) == "&nbsp;this&nbsp;is&nbsp;a&nbsp;test&nbsp;", helper.spacing(var) )
end

return test
