local HttpClient = require "charon.net.HttpClient"

local test = {}

test['return url redirect'] = function()
  local client = HttpClient.new("http://www.objectdata.com.br")
  local result = client:performGet()
  assert( result:contains("<title>Object Data") == true )
  assert( client:urlRedirect() == "http://www.objectdata.com.br/home", client:urlRedirect() )
end

test['return nil url redirect'] = function()
  local client = HttpClient.new("http://www.google.com.br")
  local result = client:performGet()
  assert( result:contains("<title>Google</title>") == true )
  assert( client:urlRedirect() == nil, client:urlRedirect() )
end

return test
