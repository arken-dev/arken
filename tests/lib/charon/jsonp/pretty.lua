local json  = require 'charon.jsonp'
local tests = {}

tests.should_json_encode_pretty = function()
  json.pretty{ id = 1 }
end

return tests
