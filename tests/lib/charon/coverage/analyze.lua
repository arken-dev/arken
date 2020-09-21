local coverage = require('arken.coverage')
local test = {}

test.should_return_a_table = function()
  local data = coverage.analyze("@lib/arken/coverage.lua")
  assert( type(data) == 'table' )
end

test.should_return_a_total_coverage = function()
  local data = coverage.analyze("@lib/arken/coverage.lua")
  assert(type(data.coverage) == 'number')
end

test.should_return_a_lines = function()
  local data = coverage.analyze("@lib/arken/coverage.lua")
  assert(type(data.lines) == 'table')
end

test.should_return_a_lines = function()
  local data = coverage.analyze("@lib/arken/coverage.lua")
  assert(data.file_name == os.abspath("lib/arken/coverage.lua"))
end

return test
