local json  = require 'charon.jsonp'
local tests = {}

tests.should_json_encode_pretty = function()
  json.pretty({ id = 1, teste = {''} })
  local pretty = json.pretty
  local teste = json.pretty{ id = 1 }
  pcall(json.pretty, nil)
  json.pretty {
    id = 1,
    teste = 'teste'
  }
end

return tests
