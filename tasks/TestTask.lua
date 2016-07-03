OBERON_ENV = os.getenv("OBERON_ENV") or "test"

local colorize = require 'colorize'
local test     = require 'test'
local TestTask = Class.new("TestTask")

function string:escape()
  local tmp = self
  tmp = tmp:replace("'", "\\'")
  tmp = tmp:replace('"', '\\"')
  return tmp
end

function TestTask:console(params)
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

  results = test.execute(arg)

  for file_name, result in pairs(results) do
    print(file_name)
    for description, result in pairs(result) do
      --print(result.status)
      count[result.status] = count[result.status] + 1
      print(colorize.format(description, color[result.status]))
      if result.msg:len() > 0  then
        print(result.msg)
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

end

function TestTask:notify(params)
  local time = os.microtime()
  local file = arg[2]
  while true do
    t = QDateTime.currentMSecsSinceEpoch()
    package.reload()
    results = test.execute({file})

    buffer = ""
    titulo = ""

    count = {}
    count.ok      = 0
    count.err     = 0
    count.fail    = 0
    count.pending = 0

    for file_name, result in pairs(results) do
      if titulo:len() > 0 then
        titulo = titulo, ', '
      end
      titulo = titulo .. file_name
    end

    for file_name, result in pairs(results) do
      for description, result in pairs(result) do
        count[result.status] = count[result.status] + 1
        if result.status ~= 'ok' then
          buffer = buffer .. description .. ' '
          if result.msg and result.msg:len() > 0  then
            buffer = buffer .. ' ' .. result.status .. '\n'
            buffer = '\n' .. buffer .. result.msg .. '\n'--print(result.msg)
          end
        end
      end
      print("")
      rodape = "\n"
      for i, v in pairs(count) do
        if rodape:len() > 0 then
          rodape = rodape .. ', '
        end
        rodape = rodape .. v .. " " .. i
      end
      buffer = buffer .. '\n' .. rodape .. "\n"
      buffer = buffer .. 'Time: ' .. tostring((QDateTime.currentMSecsSinceEpoch() - t) / 1000.0)
      print(buffer:replace('\n', '8'))
      icon = "error"
      --buffer = buffer:replace('\n\n', '')
      print("notify-send -t 10000 " .. "'" .. titulo .. "' '\"" .. buffer:escape() .. "\"")
      os.execute("notify-send -t 10000 " .. "'" .. titulo .. "' \"" .. buffer:escape() .. "\"")
    end
    while os.ctime(file) <= time  do
      os.sleep(0.15)
    end
    os.sleep(0.10)
    time = os.ctime(file)
  end
end

function TestTask:create()
  local template = require 'template'

  print("Digite o nome da classe:")
  self.class_name = io.read()
  local path = 'specs/models/' .. self.class_name:replace('.', '/')
  os.mkpath( path )

  print("Digite o nome do método:")
  local metodo = io.read()
  local test   = path .. '/' .. metodo .. '.lua'
  local file   = QFile.new(test)
  local tpl    = OBERON_PATH .. "/tasks/templates/test/create/metodo.tpl"
  local buffer = template.execute(tpl, self)
  file:open({"WriteOnly"})
  file:write(buffer)
  file:close()

  print('mkpath '.. path)
  print('test create ' .. test)
end

return TestTask
