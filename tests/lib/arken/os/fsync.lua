local mvm  = require('arken.mvm')
local test = {}

test['should return true when file exists'] = function()
  local fileName = mvm.path() .. '/TODO'
  assert( os.fsync( fileName ) == true )
end

test['should return false when file does not exist'] = function()
  local fileName = mvm.path() .. '/this-file-does-not-exist'
  assert( os.fsync( fileName ) == false )
end

return test
