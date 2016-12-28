local helper = require 'charon.Helper'
local test   = {}

test.return_yes = function()
  assert( helper:boolFormat(true) == "yes" )
end

test.return_no = function()
  assert( helper:boolFormat(false) == "no" )
end

test.return_sim = function()
  assert( helper:boolFormat(true, 'sim') == "sim" )
end

test.return_no = function()
  assert( helper:boolFormat(false, 'sim', 'não') == "não" )
end

return test
