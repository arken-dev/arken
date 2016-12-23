CHARON_ENV = os.getenv("CHARON_ENV") or "test"

local QDirIterator = require 'QDirIterator'
local QFileInfo    = require 'QFileInfo'
local test         = require 'charon.test'
local template     = require 'charon.template'
local coverage     = require 'charon.coverage'
local start        = os.microtime()
local files        = {}

-------------------------------------------------------------------------------
-- TRITON START
-------------------------------------------------------------------------------

function triton_start()
  local iterator = QDirIterator.new('./lib', {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    print(fileInfo:filePath())
    if fileInfo:filePath():endsWith(".lua") then
      local filePath = fileInfo:filePath()
      print(filePath)
      table.insert(files, filePath)
      triton.enqueue(filePath)
    end
  end
end

function triton_run(fileName)
  local tests     = {}
  local dirName   = fileName:gsub(".lua", ""):replace("./", "./tests/")
  print('dirName')
  print(dirName)
  local modelName = dirName:gsub("./tests/lib/", ""):gsub("/", ".")
  local iterator  = QDirIterator.new(dirName)
  package.loaded[modelName] = nil

  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if fileInfo:filePath():endsWith(".lua") then
      table.insert(tests, fileInfo:filePath())
    end
  end

  coverage.reset()
  coverage.start()
  local results = test.execute(tests)
  coverage.stop()

  for fileName, result in pairs(results) do
    for description, result in pairs(result) do
      triton.addOk()
      if result.status ~= 'ok' then
        local buffer = description .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          buffer = buffer .. tostring(result.msg) .. '\n'
        end
        triton.append('message', fileName .. '\n' .. buffer)
      end

      if result.status == 'failure' then
        triton.addError()
      end

      if result.status == 'pending' then
        triton.addPending()
      end
    end
  end

  local dir     = 'coverage'
  local file    = '@' .. fileName:mid(2, -1)
  local tpl     = CHARON_PATH .. "/lib/charon/coverage/templates/file.html"
  local dump    = coverage.dump()
  local data    = coverage.analyze(file)
  local buffer  = template.execute(tpl, data)
  local file    = io.open((dir .. "/" .. data.file_name:replace("/", "-") .. '.html'), "w")
  file:write(buffer)
  file:close()

  file = io.open((dir .. "/" .. data.file_name:replace("/", "-") .. '.json'), "w")
  --file:write(require('JSON'):encode_pretty(data))
  file:write(json.encode(data))
  file:close()
end

-------------------------------------------------------------------------------
-- TRITON STOP
-------------------------------------------------------------------------------

function triton_stop()
  for i, file in ipairs(files) do
    files[i] = 'coverage/@' .. file:mid(2, -1):replace('/', '-') .. '.json'
  end
  print('')
  local dir    = 'coverage'
  local tpl    = CHARON_PATH .. "/lib/charon/coverage/templates/index.html"
  local data   = {files = files, time = (os.microtime() - start), total = triton.ok()}
  local buffer = template.execute(tpl, data)

  local file   = io.open((dir .. "/" .. 'index.html'), "w")
  file:write(buffer)
  file:close()

  local result = "%i tests, %i pendings, %i failures"
  print('\n' .. triton.result('message'))
  print(string.format(result, triton.total('test'), triton.total('failure'), triton.total('pending')))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end
