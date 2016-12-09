local coverage = require 'charon.coverage'
local should   = require 'test.should'
local test   = {}

coverage.start()
loadfile('specs.coverage.1-example.example')
coverage.stop()

test['should return the table type of name file'] = function()
  local result = coverage.dump()
  assert(type(result['@./specs/coverage/1-example.lua']) == 'table')
end

test['should return flag 1 for line 1'] = function()
  local analyze = coverage.analyze('./specs/coverage/1-example.lua')
  assert(analyze[1].line == 'flag = true')
  assert(analyze[2].flag == 1)
end

test['should return flag -1 for line 5'] = function()
  local analyze = coverage.analyze('./specs/coverage/1-example.lua')
  assert(analyze[5].line == 'else')
  assert(analyze[5].flag == -1)
end

test['should return flag nil for line 6'] = function()
  local analyze = coverage.analyze('./specs/coverage/1-example.lua')
  assert(analyze[6].line == "  var = 'false..'")
  assert(analyze[6].flag == nil)
end

return test
