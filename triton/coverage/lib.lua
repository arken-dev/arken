ARKEN_ENV = os.getenv("ARKEN_ENV") or "test"

local test     = require 'arken.test'
local template = require 'arken.template'
local coverage = require 'arken.coverage'
local json     = require 'arken.jsonp'
local mvm      = require 'arken.mvm'
local start    = os.microtime()
local files    = {}
local M = {}

-------------------------------------------------------------------------------
-- START
-------------------------------------------------------------------------------

function M.start()
  local dir = 'coverage'
  if not os.exists(dir) then
    os.mkdir(dir)
  end

  local list = os.glob('./lib', true)
  for i = 1, list:size() do
    local filePath = list:at(i)
    if filePath:endsWith(".lua") then
      print(filePath)
      table.insert(files, filePath)
      triton.enqueue(filePath)
    end
  end
end

-------------------------------------------------------------------------------
-- RUN
-------------------------------------------------------------------------------

function M.run(fileName)
  local tests     = {}
  local dirName   = fileName:gsub(".lua", ""):replace("./", "./tests/")
  if not os.exists(dirName) then
    triton.append("message", string.format("warning: directory %s not exists\n", dirName))
  end
  local modelName = dirName:gsub("./tests/lib/", ""):gsub("/", ".")
  local list = os.glob(dirName)

  if modelName == 'arken.coverage' then
    package.loaded[modelName] = nil
  end

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
    for description, result in pairs(result) do
      triton.count('tests')
      if result.status ~= 'ok' then
        local buffer = description .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          buffer = buffer .. tostring(result.msg) .. '\n'
        end
        triton.append('message', fileName .. '\n' .. buffer)
      end

      triton.count(result.status)
    end
  end

  local dir     = 'coverage'
  local file    = fileName
  local tpl     = mvm.path() .. "/lib/arken/coverage/templates/file.html"
  local dump    = coverage.dump()
  local data    = coverage.analyze(fileName)
  local buffer  = template.execute(tpl, data )
  local file    = io.open((dir .. "/" .. fileName:replace("/", "-") .. '.html'), "w")
  file:write(buffer)
  file:close()
  file = io.open((dir .. "/" .. fileName:replace("/", "-") .. '.json'), "w")
  file:write(json.encode(data))
  file:close()
end

-------------------------------------------------------------------------------
-- STOP
-------------------------------------------------------------------------------

function M.stop()
  print('')
  local dir    = 'coverage'
  local tpl    = mvm.path() .. "/lib/arken/coverage/templates/index.html"
  local data   = {files = files, time = (os.microtime() - start), total = triton.total('tests')}
  local buffer = template.execute(tpl, data)

  local file   = io.open((dir .. "/" .. 'index.html'), "w")
  file:write(buffer)
  file:close()

  local result = "%i tests, %i pendings, %i failures"
  print('\n' .. triton.result('message'))
  print(string.format(result, triton.total('tests'), triton.total('failure'), triton.total('pending')))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end

return M
