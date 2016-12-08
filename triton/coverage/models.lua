OBERON_ENV = os.getenv("OBERON_ENV") or "test"

local test     = require 'test'
local template = require 'template'
local coverage = require 'oberon.coverage'
local start    = os.microtime()
local files    = {}

-------------------------------------------------------------------------------
-- TRITON START
-------------------------------------------------------------------------------

function triton_start()
  local iterator = QDirIterator.new('./app/models', {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
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
  local dirName   = fileName:gsub(".lua", ""):gsub("app", "tests")
  local modelName = dirName:gsub("./tests/models/", ""):gsub("/", ".")
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
      triton.count('test')
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
  local file    = '@' .. fileName:mid(2, -1)
  local tpl     = OBERON_PATH .. "/lib/oberon/coverage/templates/file.html"
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
  local tpl    = OBERON_PATH .. "/lib/oberon/coverage/templates/index.html"
  local data   = {files = files, time = (os.microtime() - start), total = triton.total('tests')}
  local buffer = template.execute(tpl, data)

  local file   = io.open((dir .. "/" .. 'index.html'), "w")
  file:write(buffer)
  file:close()

  local result = "%i tests, %i pendings, %i failures"
  print('\n' .. triton.result('message'))
  print(string.format(result, triton.total('test'), triton.total('failure'), triton.total('pending')))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end
