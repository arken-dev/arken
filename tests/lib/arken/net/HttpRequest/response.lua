local HttpEnv = require "arken.net.HttpEnv"
local HttpRequest = require "arken.net.HttpRequest"
local mvm  = require('arken.mvm')
local test = {}

test.should_append_headers= function()
  local request = HttpRequest.new()
  request:cookie('box', 'full')
  local headers = {}
  request:response(headers)
  assert(headers[1] == 'Set-Cookie: box=full', headers[1])
end

test.should_insert_cookie_session = function()
  _G.cache = {}
  _G.cache.insert = function()
  end
  local header = os.read(mvm.path() .. '/tests/lib/arken/net/HttpEnv/example-2.txt')
  local env = HttpEnv.new(header)
  local request = HttpRequest.new{ _env = env }
  request:session()
  local headers = {}
  request:response(headers)
  assert(headers[1]:startsWith('Set-Cookie: arken_session_id=') == true, headers[1])
end


return test
