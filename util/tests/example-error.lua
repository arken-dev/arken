local test = {}

test.should_failure_for_invalid_test = function()
  local value = false
  assert( value == true )
end

test.should_failure_with_table = function()
  local myerrors = { name = 'name is invalid' }
  error(myerrors)
end

local errorWithTraceback = function()
  local errors = { name = 'name is invalid' }
  errors.traceback = debug.traceback()
  error(errors)
end

test.should_failure_with_traceback = function()
  errorWithTraceback()
end

return test
