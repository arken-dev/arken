local HttpClient = require "arken.net.HttpClient"

local test = {}

test['parse HTTP/1.1'] = function()
  local path = debug.getinfo(1).source:sub(2):prefix(".lua")
  local data = os.read(path .. '/data-1.txt')
  local status = HttpClient.parseStatus(data)
  assert( status == 200, status )
end

test['parse HTTP/1.1 space before'] = function()
  local path = debug.getinfo(1).source:sub(2):prefix(".lua")
  local data = os.read(path .. '/data-1-before-space.txt')
  local status = HttpClient.parseStatus(data)
  assert( status == 200, status )
end

test['parse HTTP/1.1 after before'] = function()
  local path = debug.getinfo(1).source:sub(2):prefix(".lua")
  local data = os.read(path .. '/data-1-after-space.txt')
  local status = HttpClient.parseStatus(data)
  assert( status == 200, status )
end

test['parse HTTP/1.1 around before'] = function()
  local path = debug.getinfo(1).source:sub(2):prefix(".lua")
  local data = os.read(path .. '/data-1-around-space.txt')
  local status = HttpClient.parseStatus(data)
  assert( status == 200, status )
end

test['parse HTTP/2'] = function()
  local path = debug.getinfo(1).source:sub(2):prefix(".lua")
  local data = os.read(path .. '/data-2.txt')
  local status = HttpClient.parseStatus(data)
  assert( status == 403, status )
end

test['parse product api example'] = function()
  local path = debug.getinfo(1).source:sub(2):prefix(".lua")
  local data = os.read(path .. '/data-update-produto.txt')
  local status = HttpClient.parseStatus(data)
  assert( status == 200, status )
end


return test
