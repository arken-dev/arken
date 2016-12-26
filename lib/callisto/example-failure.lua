local test = {}

test.this_is_failure = function()
  error( 'failure' )
end

return test
