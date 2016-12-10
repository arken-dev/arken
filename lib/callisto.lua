CHARON_ENV = os.getenv("CHARON_ENV") or "test"

local test  = require 'test'

callisto = function(file)
  local t = QDateTime.currentMSecsSinceEpoch()
  package.reload()
  local results = test.execute({file})

  local buffer = ""
  local titulo = ""
  local icon   = "ok"

  count = {}
  count.ok      = 0
  count.failure = 0
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
        buffer = buffer .. '<b>' .. description .. '</b>' .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          --buffer = buffer .. ' ' .. result.status .. '\n'
          buffer = buffer .. tostring(result.msg) .. '\n\n'--print(result.msg)
        end
      end
    end

    if count.pending > 0 or count.ok == 0 then
      icon = "warning"
    end

    if count.failure > 0 then
      icon = "failure"
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
    buffer = buffer .. rodape
    buffer = buffer .. '\n\nFinished in ' .. tostring((QDateTime.currentMSecsSinceEpoch() - t) / 1000.0) .. ' seconds'
    print(buffer)
    return icon, buffer:swap('\n', '<br>')
end
