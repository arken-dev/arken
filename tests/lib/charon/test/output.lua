local ctest = require 'charon.test'
local test  = {}

test.should_output_function_is_io_write = function()
  assert(ctest.output == io.write)
end

return test
