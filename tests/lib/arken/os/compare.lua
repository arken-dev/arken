local mvm  = require('arken.mvm')
local test = {}

test['should return true for identical file'] = function()
  local fileName = mvm.path() .. '/TODO'
  assert( os.compare( fileName, fileName ) == true )
end

test['should return false for not egual content file'] = function()
  local fileName1 = mvm.path() .. '/TODO'
  local fileName2 = mvm.path() .. '/LICENSE'
  assert( os.compare( fileName1, fileName2 ) == false )
end

return test
