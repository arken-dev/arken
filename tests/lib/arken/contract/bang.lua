local contract = require('arken.contract')
local test     = {}

test.should_bang_errors_and_generage_traceback = function()
  local model  = {}
  model.errors = {}
  model.errors.total = 'is a total error'
  local result, message = pcall(contract.bang, model.errors)
  assert( result == false )
  assert( type(message) == 'table' )
  assert( message.total:contains("is a total error") == true )
  assert( type(message.traceback) == 'string' )
end

return test
