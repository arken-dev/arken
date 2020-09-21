local test = {}
local callisto = require 'callisto'

test.should_error_not_exists_file = function()
  local test = require('arken.test')
  test.output = function() end
  local result, message = pcall(callisto, "util/callisto/example-not-exists.lua")
  test.output = io.write
  assert(result == false)
  assert(message:contains("util/callisto/example-not-exists.lua not exists"), message)
end

test.should_result_ok = function()
  local test = require('arken.test')
  test.output = function() end
  local icon, result = callisto("util/callisto/example-ok.lua")
  test.output = io.write
  assert(icon == "ok", icon)
end

test.should_result_warning = function()
  local test = require('arken.test')
  test.output = function() end
  local icon, result = callisto("util/callisto/example-warning.lua")
  test.output = io.write
  assert(icon == "warning", icon)
end

test.should_result_failure = function()
  local test = require('arken.test')
  test.output = function() end
  local icon, result = callisto("util/callisto/example-failure.lua")
  test.output = io.write
  assert(icon == "failure", icon)
  assert(result:contains("util/callisto/example-failure.lua:4: failure"), result)
end

return test
