local rhea = require 'rhea'
local json = require 'charon.json'
local test = {}

-------------------------------------------------------------------------------
-- OUTPUT TEST

local result = ''

function output(value)
  result = value
end

rhea.output = output

-------------------------------------------------------------------------------
-- MODULE TEST

local MyRheaTask = {}
MyRheaTask.help  = {}

MyRheaTask.help.prepapre = [[
  prepare database for development
]]

MyRheaTask.prepare = function()
end

test.should_return_string_help = function()
  rhea.help(MyRheaTask)
  assert( result == 'prepapre # prepare database for development\n', result )
end

return test
