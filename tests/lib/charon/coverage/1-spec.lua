local coverage = require 'charon.coverage'
local json = require('charon.json')
local test = {}
local file_coverage = '@' .. CHARON_PATH .. '/lib/charon/coverage/examples/1-example.lua'

coverage.start()
require('charon.coverage.examples.1-example')
coverage.stop()

test['should return the table type of name file'] = function()
  local result = coverage.dump()
  local buffer = ""
  for k, v in pairs(result) do
    buffer = buffer .. k
  end
  assert(type(result[file_coverage]) == 'table')
end

test['should return flag 1 for line 6'] = function()
  local analyze = coverage.analyze(file_coverage).lines
  assert(analyze[6].src == 'flag = true', json.encode(analyze[6]))
  assert(analyze[6].flag == 1, json.encode(analyze[6]))
end

test['should return flag -1 for line 10'] = function()
  local analyze = coverage.analyze(file_coverage).lines
  assert(analyze[10].src == 'else', analyze[10])
  assert(analyze[10].flag == -1, analyze[10])
end

test['should return flag nil for line 11'] = function()
  local analyze = coverage.analyze(file_coverage).lines
  assert(analyze[11].src == "  var = 'false..'")
  assert(analyze[11].flag == nil)
end

return test
