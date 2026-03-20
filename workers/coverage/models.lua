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

function M.start(worker)
  local prepare = 'worker/coverage/prepare.lua'
  if os.exists(prepare) then
    dofile(prepare)
  end

  local dir = 'coverage'
  if not os.exists(dir) then
    os.mkdir(dir)
  end

  local list = os.find('./app/models', '\\.lua$', true)
  for i = 1, list:size() do
    local filePath = list:at(i)
    table.insert(files, filePath)
    worker:enqueue(filePath)
  end
end

-------------------------------------------------------------------------------
-- BEFORE
-------------------------------------------------------------------------------

function M.before(worker)
  local ActiveRecord = require('arken.ActiveRecord')
  ActiveRecord.loadConfig{ number = worker:number() }
  if worker:params().progress then
    test.output = function() end
  end
end

-------------------------------------------------------------------------------
-- RUN
-------------------------------------------------------------------------------

function M.run(node, fileName, params)
  local tests     = {}
  local shared    = node:master():shared()
  local dirName   = fileName:replace(".lua", ""):replace("/app", "/tests")
  local modelName = dirName:replace("./tests/models/", ""):replace("/", ".")

  local list = os.find(dirName, '\\.lua$')
  package.loaded[modelName] = nil

  for i = 1, list:size() do
    local filePath = list:at(i)
    table.insert(tests, filePath)
  end

  coverage.reset()
  coverage.start()
  local results = test.execute(tests, flag)
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

  if os.mem()/1024 > 800 then
    shared:append('debug-mem', string.format("%s memória: %s MB\n", fileName, os.mem()/1024))
  end

  collectgarbage("collect")
end

-------------------------------------------------------------------------------
-- AFTER
-------------------------------------------------------------------------------

function M.after(worker)
end

-------------------------------------------------------------------------------
-- TRITON STOP
-------------------------------------------------------------------------------

function M.stop(worker)
  --os.exit()
  local shared = worker:shared()
  local dir    = 'coverage'
  local tpl    = mvm.path() .. "/lib/arken/coverage/templates/index.html"
  local data   = {files = files, time = (os.microtime() - start), total = shared:getNumber('tests') }
  local buffer = template.execute(tpl, data)

  local file   = io.open((dir .. "/" .. 'index.html'), "w")
  file:write(buffer)
  file:close()

  local result = "%i tests, %i pendings, %i failures, %.4f%% coverage"
  print('\n' .. shared:getString('message'))
  print(string.format(result, shared:getNumber('tests'), shared:getNumber('pending'), shared:getNumber('failure'), data.coverage))
  print(string.format("Finished in %.2f seconds mem usage %s", os.microtime() - start, os.mem()))
  if worker:params().progress then
    print(shared:getString('debug-mem'))
  end
end

return M
