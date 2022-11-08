local mvm  = require('arken.mvm')
local json = require('arken.json')

local instance = mvm.current()
if instance then
  instance:shared():name('script/server')
end

local HttpServer = require('arken.net.HttpServer')
local config     = json.file('config/server.json')
local server     = HttpServer.new(config.address, config.port)

if config.service or os.pwd():contains('objectdata.com.br') then
  server:addService('app/services')
end

server:setThreads(config.threads)
server:setPid(config.pid)
server:setDispatcher("lib.dispatcher")
server:start()
