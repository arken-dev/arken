ARKEN_ENV = os.getenv("ARKEN_ENV") or "test"

local test  = require 'arken.test'
local start = os.microtime()
local M = {}

-------------------------------------------------------------------------------
-- START
-------------------------------------------------------------------------------

function M:start(params)
  local path = params.path
  local list = os.find(path, ".lua$", true)
  for i = 1, list:size() do
    local filePath = list:at(i)
    self:enqueue(filePath)
  end
end

-------------------------------------------------------------------------------
-- BEFORE
-------------------------------------------------------------------------------

function M:before(worker)
  if os.exists( os.pwd() .. '/config/active_record.json' ) then
    local ActiveRecord = require('arken.ActiveRecord')
    ActiveRecord.loadConfig{ number = self:number() }
  end
end

-------------------------------------------------------------------------------
-- RUN
-------------------------------------------------------------------------------

function M:run(fileName)

  local shared  = self:shared()
  local results = test.execute({ fileName })
  local titulo  = ""

  for file_name, result in pairs(results) do
    if titulo:len() > 0 then
      titulo = titulo, ', '
    end
    titulo = titulo .. file_name:replace('tests/models/', '')
  end

  for file_name, result in pairs(results) do
    local count  = 0
    local status = {}
    for description, result in pairs(result) do

      count = count + 1

      if result.status ~= 'ok' then
        local buffer = description .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          buffer = buffer .. tostring(result.msg) .. '\n'
        end
        shared:append('message', fileName .. '\n' .. buffer)
      end

      status[result.status] = status[result.status] or 0
      status[result.status] = status[result.status] + 1
    end
    shared:increment('tests', count)
    for status, total in pairs(status) do
      shared:increment(status, total)
    end
  end
end

function M:stop()
  local shared = self:shared()
  local result = "%i tests, %i pendings, %i failures"
  print('\n' .. shared:getString('message'))
  print(string.format(result, shared:getNumber('tests'), shared:getNumber('failure'), shared:getNumber('pending')))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end

return M
