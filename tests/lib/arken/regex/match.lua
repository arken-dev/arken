local regex = require 'arken.regex'
local test  = {}

test['should be true for (g|p|r)ato'] = function()
  assert(regex.match("rato", "(g|p|r)ato") == true)
  assert(regex.match("gato", "(g|p|r)ato") == true)
  assert(regex.match("pato", "(g|p|r)ato") == true)
  assert(regex.match("iato", "(g|p|r)ato") == false)
end

test['should be false for (g|p|r)ato'] = function()
  assert(regex.match("tato", "(g|p|r)ato") == false)
  assert(regex.match("fato", "(g|p|r)ato") == false)
  assert(regex.match("mato", "(g|p|r)ato") == false)
end


return test
