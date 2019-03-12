local test = {}

test['should return true for identical file'] = function()
  local fileName = CHARON_PATH .. '/TODO'
  assert( os.compare( fileName, fileName ) == true )
end

test['should return false for not egual content file'] = function()
  local fileName1 = CHARON_PATH .. '/TODO'
  local fileName2 = CHARON_PATH .. '/LICENSE'
  assert( os.compare( fileName1, fileName2 ) == false )
end

return test
