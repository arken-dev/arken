ARKEN_ENV = os.getenv("ARKEN_ENV") or "test"

local ActiveRecord = require "arken.ActiveRecord"
local test         = require 'arken.test'
local template     = require 'arken.template'
local coverage     = require 'arken.coverage'
local mvm          = require 'arken.mvm'
local start        = os.microtime()
local files        = {}
local M = {}

-------------------------------------------------------------------------------
-- START
-------------------------------------------------------------------------------

function M.start(triton)
  local prepare = 'triton/coverage/prepare.lua'
  if os.exists(prepare) then
    dofile(prepare)
  end

  local dir = 'coverage'
  if not os.exists(dir) then
    os.mkdir(dir)
  end

  local list = os.glob('./app/models', true)
  for i = 1, list:size() do
    local filePath = list:at(i)
    if filePath:endsWith(".lua") then
      table.insert(files, filePath)
      triton:enqueue(filePath)
    end
  end
end

-------------------------------------------------------------------------------
-- BEFORE
-------------------------------------------------------------------------------

function M.before(triton)
  local ActiveRecord = require('arken.ActiveRecord')
  ActiveRecord.loadConfig{ triton = triton:number() }
end

-------------------------------------------------------------------------------
-- RUN
-------------------------------------------------------------------------------

function M.run(triton, fileName)
  local tests     = {}
  local shared    = triton:shared()
  local dirName   = fileName:replace(".lua", ""):replace("/app", "/tests")
  local modelName = dirName:replace("./tests/models/", ""):replace("/", ".")

  local list = os.glob(dirName)
  package.loaded[modelName] = nil

  for i = 1, list:size() do
    local filePath = list:at(i)
    if filePath:endsWith(".lua") then
      table.insert(tests, filePath)
    end
  end

  coverage.reset()
  coverage.start()
  local results = test.execute(tests)
  coverage.stop()

  for fileName, result in pairs(results) do
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

  local dir     = 'coverage'
  local file    = fileName:mid(3, -1)
  local tpl     = mvm.path() .. "/lib/arken/coverage/templates/file.html"
  local dump    = coverage.dump()
  local data    = coverage.analyze(file)
  local buffer  = template.execute(tpl, data)
  local file    = io.open((dir .. "/" .. fileName:replace("/", "-") .. '.html'), "w")
  file:write(buffer)
  file:close()

  file = io.open((dir .. "/" .. fileName:replace("/", "-") .. '.json'), "w")

  file:write(json.encode(data))

  file:close()
end

-------------------------------------------------------------------------------
-- AFTER
-------------------------------------------------------------------------------

function M.after(triton)
end

-------------------------------------------------------------------------------
-- TRITON STOP
-------------------------------------------------------------------------------

function M.stop(triton)
  --os.exit()
  local shared = triton:shared()
  local dir    = 'coverage'
  local tpl    = mvm.path() .. "/lib/arken/coverage/templates/index.html"
  local data   = {files = files, time = (os.microtime() - start), total = shared:getNumber('tests') }
  local buffer = template.execute(tpl, data)

  local file   = io.open((dir .. "/" .. 'index.html'), "w")
  file:write(buffer)
  file:close()

  local result = "%i tests, %i pendings, %i failures, %.4f%% coverage"
  print('\n' .. shared:getString('message'))
  print(string.format(result, shared:getNumber('tests'), shared:getNumber('failure'), shared:getNumber('pending'), data.coverage))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end

return M
