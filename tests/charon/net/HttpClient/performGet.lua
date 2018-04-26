local HttpClient = require "charon.net.HttpClient"

local test = {}

test['return content from url'] = function()
  local client = HttpClient.new("http://www.objectdata.com.br")
  local result = client:performGet()
  assert( result:contains("<title>Object Data") == true )
end

return test
