local ctest = require 'arken.test'
local json  = require 'arken.json'
local test  = {}


test.should_status_ok_for_valid_test = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/exemple-1.lua')
  assert(message.should_assert_true.status == 'ok')
  ctest.output = io.write
end

test.should_status_failure_for_invalid_test = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/example-error.lua')
  assert(message.should_failure_for_invalid_test.status == 'failure', json.encode(message))
  ctest.output = io.write
end

test.should_status_failure_for_invalid_test_with_traceback = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/example-error.lua')
  local status = message.should_failure_with_traceback.status
  local msg = message.should_failure_with_traceback.msg
  assert(status == 'failure', status)
  assert(msg:contains("example-error.lua:20: in function") == true, json.encode(message))
  ctest.output = io.write
end

test.should_error_if_file_not_exists = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/example-not-exists.lua')
  local status  = message['util/tests/example-not-exists.lua'].status
  local msg = message['util/tests/example-not-exists.lua'].msg
  assert(result == true)
  assert(status == 'failure', json.encode(message))
  assert(msg:startsWith("cannot open util/tests/example-not-exists.lua") == true, json.encode(message))
  ctest.output = io.write
end

test.should_return_empty_tests_error = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/example-empty.lua')
  assert(result == false)
  assert(message:contains('empty tests...') == true, message)
  ctest.output = io.write
end

test.should_return_error_after = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/examplo-error-after.lua')
  assert(result == true)
  assert(message.after.msg:contains('error after...') == true, json.encode(message))
  ctest.output = io.write
end

test.should_return_error_after_all = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/example-error-afterAll.lua')
  assert(result == false)
  assert(message:contains('error after all') == true, json.encode(message))
  ctest.output = io.write
end

test.should_return_error_before_all = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/example-error-beforeAll.lua')
  assert(result == false)
  assert(message:contains('error before all') == true, json.encode(message))
  ctest.output = io.write
end


test.should_return_pending = function()
  ctest.output = function() end
  local result, message = pcall(ctest.process, 'util/tests/example-pending.lua')
  local status = message.should_return_pending.status
  local msg = message.should_return_pending.msg
  assert( status == 'pending')
  assert( msg == 'my pending...', msg )
  ctest.output = io.write
end

return test
