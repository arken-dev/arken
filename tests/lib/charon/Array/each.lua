local Array = require('charon.Array')
local test  = {}

test.should_each_element_of_array = function()
  local list = Array.new{
    { id = 1, name = 'John' },
    { id = 2, name = 'Steve' }
  }
  local buffer = ""
  for user in list:each() do
    if buffer:len() > 0 then
      buffer = buffer .. ', '
    end
    buffer = buffer .. user.name
  end
  assert( buffer == 'John, Steve' )
end

return test
