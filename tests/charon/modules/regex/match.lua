require 'charon.regex'

local test = {}

test['should be true for (g|p|r)ato'] = function()
  assert(regex.match("rato", "(g|p|r)ato") == true)
  assert(regex.match("gato", "(g|p|r)ato") == true)
  assert(regex.match("pato", "(g|p|r)ato") == true)
  assert(regex.match("iato", "(g|p|r)ato") == false)
end

return test
