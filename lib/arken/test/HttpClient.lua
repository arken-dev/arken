local HttpClient = Class.new("arken.test.HttpClient")
local template   = require("arken.template")
local HttpEnv    = require "arken.net.HttpEnv"
local mvm        = require "arken.mvm"


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
  self.headers = self.headers or {}
  local env = template.execute(mvm.path() .. "/lib/arken/test/HttpClient/post.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

function HttpClient:performGet()
  self.headers = self.headers or {}
  local env = template.execute(mvm.path() .. "/lib/arken/test/HttpClient/get.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

function HttpClient:performPut()
  self.headers = self.headers or {}
  local env = template.execute(mvm.path() .. "/lib/arken/test/HttpClient/put.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

function HttpClient:performDelete()
  self.headers = self.headers or {}
  local env = template.execute(mvm.path() .. "/lib/arken/test/HttpClient/delete.tpl", self, {})
  if self.verbose then
    print(env)
  end
  local status, headers, result = dispatch(HttpEnv.new(env))
  self._status = status
  return result;
end

return HttpClient
