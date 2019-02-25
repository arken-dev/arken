local HttpClient = Class.new("charon.test.HttpClient")
local template   = require("charon.template")
local HttpEnv    = require "charon.net.HttpEnv"


require('dispatch')


HttpClient.inherit = function(class)

  class.newOld = class.new

  class.new = function(url)
    return class.newOld{ _url = url, _body = "" }
  end

end

function HttpClient:status()
  return self._status
end

function HttpClient:appendHeader(header)
  self.headers = self.headers or {}
  table.insert(self.headers, header)
end

function HttpClient:setVerbose(flag)
  self.verbose = flag
end

function HttpClient:setBody(body)
  self._body = body .. "\r\n"
end

function HttpClient:body()
  return self._body
end

function HttpClient:performPost()
  local env = template.execute(CHARON_PATH .. "/lib/charon/test/HttpClient/post.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

function HttpClient:performGet()
  local env = template.execute(CHARON_PATH .. "/lib/charon/test/HttpClient/get.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

function HttpClient:performPut()
  local env = template.execute(CHARON_PATH .. "/lib/charon/test/HttpClient/put.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

function HttpClient:performDelete()
  local env = template.execute(CHARON_PATH .. "/lib/charon/test/HttpClient/delete.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

return HttpClient
