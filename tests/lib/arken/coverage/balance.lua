local coverage = require('arken.coverage')
local test     = {}

test.should_return_zero_when_parenthesis_are_closed = function()
  assert(coverage.balance('local texto = string.format("%s", valor)') == 0)
end

test.should_return_one_when_parenthesis_is_open = function()
  assert(coverage.balance('local texto = string.format(') == 1)
end

test.should_ignore_parenthesis_inside_string = function()
  assert(coverage.balance([[local texto = ")"]]) == 0)
end

test.should_ignore_parenthesis_inside_comment = function()
  assert(coverage.balance('local texto = 1 -- string.format(') == 0)
end

test.should_ignore_parenthesis_inside_long_string = function()
  local row = "local list = self.backtrace:split([[, \"]])"
  assert(coverage.balance(row) == 0, coverage.balance(row))
end

return test
