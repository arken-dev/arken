local coverage = require 'charon.coverage'
local test = {}
local file_coverage = '@/home/ariveira/projetos/charon/lib/charon/coverage/examples/1-example.lua'
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

test['should return flag 1 for line 1'] = function()
  local analyze = coverage.analyze(file_coverage).lines
  assert(analyze[1].src == 'flag = true', json.encode(analyze[1]))
  assert(analyze[1].flag == 1, json.encode(analyze[1]))
end

test['should return flag -1 for line 5'] = function()
  local analyze = coverage.analyze(file_coverage).lines
  assert(analyze[5].src == 'else', analyze[5])
  assert(analyze[5].flag == -1, analyze[5])
end

test['should return flag nil for line 6'] = function()
  local analyze = coverage.analyze(file_coverage).lines
  assert(analyze[6].src == "  var = 'false..'")
  assert(analyze[6].flag == nil)
end

return test
