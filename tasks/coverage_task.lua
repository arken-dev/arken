local colorize = require 'colorize'
local test     = require 'test'
local template = require 'template'
local coverage = require 'oberon.coverage'

local CoverageTask = Class.new("CoverageTask")

-------------------------------------------------------------------------------
-- LINEAR
-------------------------------------------------------------------------------

function CoverageTask:linear(params)
  local iterator = QDirIterator.new('./specs', {"Subdirectories"})
  local files = {}
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if fileInfo:filePath():endsWith("_spec.lua") then
      table.insert(files, fileInfo:filePath())
    end
  end
  t = os.microtime()

  count = {}
  count.ok      = 0
  count.err     = 0
  count.fail    = 0
  count.pending = 0

  color         = {}
  color.ok      = 'green'
  color.fail    = 'red'
  color.err     = 'red_light'
  color.pending = 'yellow'

  coverage.start()
  results = test.execute(files)
  coverage.stop()

  print("\n")
  for file_name, result in pairs(results) do
    print(file_name)
    for description, result in pairs(result) do
      if result.status ~= 'ok' then
      --print(result.status)
      count[result.status] = count[result.status] + 1
      print(colorize.format(description, color[result.status]))
      if result.msg:len() > 0  then
        print(result.msg)
      end
      end
    end
    print("")
    buffer = ""
    for i, v in pairs(count) do
      if buffer:len() > 0 then
        buffer = buffer .. ', '
      end
      buffer = buffer .. v .. " " .. i
    end
    buffer = buffer .. "\n"
    buffer = buffer .. 'Time: ' .. tostring((os.microtime() - t))
    print(buffer)
  end

  print("\n\ngenerate coverage...")
  local dump  = coverage.dump()
  local dir   = 'coverage'
  local tpl   = OBERON_PATH .. "/lib/oberon/coverage/templates/file.html"
  local files = {}
  for file_name, result in pairs(dump) do
    if file_name:startsWith("@app/models") then
      local data    = coverage.analyze(file_name)
      local buffer  = template.execute(tpl, data)
      local file    = io.open((dir .. "/" .. data.file_name:replace("/", "-") .. '.html'), "w")
      table.insert(files, data)
      file:write(buffer)
      file:close()
    end
  end
  tpl = OBERON_PATH .. "/lib/oberon/coverage/templates/index.html"
  local data     = {files = files, time = (os.microtime() - t)}
  local buffer   = template.execute(tpl, data)
  local file     = io.open((dir .. "/" .. 'index.html'), "w")
  file:write(buffer)
  file:close()
end

-------------------------------------------------------------------------------
-- TREE
-------------------------------------------------------------------------------

function CoverageTask:tree(params)
  count = {}
  count.ok      = 0
  count.err     = 0
  count.fail    = 0
  count.pending = 0

  color         = {}
  color.ok      = 'green'
  color.fail    = 'red'
  color.err     = 'red_light'
  color.pending = 'yellow'

  local iterator = QDirIterator.new('./app/models', {"Subdirectories"})
  local time   = os.microtime()
  local models = {}
  local files  = {}
  local dir    = 'coverage'
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if fileInfo:filePath():endsWith(".lua") then
      table.insert(models, fileInfo:filePath())
    end
  end

  for _, fileName in pairs(models) do
    dirName  = fileName:gsub(".lua", ""):gsub("app", "specs")
    iterator = QDirIterator.new(dirName, {"Subdirectories"})
    local tests  = {}
    while(iterator:hasNext()) do
      iterator:next()
      local fileInfo = iterator:fileInfo()
      if fileInfo:filePath():endsWith(".lua") then
        table.insert(tests, fileInfo:filePath())
      end
    end

    coverage.reset()
    coverage.start()
    results = test.execute(tests)
    coverage.stop()

    if #tests > 0 then
      print("\n")
      print(fileName)
      for file_name, result in pairs(results) do
        print(file_name)
        for description, result in pairs(result) do
          if result.status ~= 'ok' then
            --print(result.status)
            count[result.status] = count[result.status] + 1
            print(colorize.format(description, color[result.status]))
            if result.msg:len() > 0  then
              print(result.msg)
            end
          end
        end
        print("")
        buffer = ""
        for i, v in pairs(count) do
          if buffer:len() > 0 then
          buffer = buffer .. ', '
        end
        buffer = buffer .. v .. " " .. i
      end
      buffer = buffer .. "\n"
      end
    end

    local file    = '@' .. fileName:mid(2, -1)
    local tpl     = OBERON_PATH .. "/lib/oberon/coverage/templates/file.html"
    local dump    = coverage.dump()
    local data    = coverage.analyze(file)
    local buffer  = template.execute(tpl, data)
    local file    = io.open((dir .. "/" .. data.file_name:replace("/", "-") .. '.html'), "w")
    table.insert(files, data)
    file:write(buffer)
    file:close()
  end

  print("\n\ngenerate coverage...")
  tpl = OBERON_PATH .. "/lib/oberon/coverage/templates/index.html"
  local data     = {files = files, time = (os.microtime() - time)}
  local buffer   = template.execute(tpl, data)
  local file     = io.open((dir .. "/" .. 'index.html'), "w")
  file:write(buffer)
  file:close()

end

return CoverageTask
