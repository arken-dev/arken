local test = {}

test.should_return_filename = function()
  local fileName = CHARON_ENV .. '/TODO'
  assert( os.basename( fileName ) == 'TODO' )
end

return test
