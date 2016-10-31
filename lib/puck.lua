OBERON_ENV = os.getenv("OBERON_ENV") or "test"

local test  = require 'test'

puck = function(file)
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
    titulo = titulo .. file_name:swap('specs/models/', '')
  end

  for file_name, result in pairs(results) do
    for description, result in pairs(result) do
      count[result.status] = count[result.status] + 1
      if result.status ~= 'ok' then
        buffer = buffer .. description .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          --buffer = buffer .. ' ' .. result.status .. '\n'
          buffer = buffer .. '\n' .. tostring(result.msg) .. '\n'--print(result.msg)
        end
      end
    end
    print("")
    rodape = ""
    for i, v in pairs(count) do
      if rodape:len() > 1 then
        rodape = rodape .. ', '
      end
        rodape = rodape .. v .. " " .. i
      end
    end
    buffer = buffer .. '\n' .. rodape
    buffer = buffer .. '\n\nFinished in ' .. tostring((QDateTime.currentMSecsSinceEpoch() - t) / 1000.0) .. ' seconds'
    print(buffer)
    return buffer
end
