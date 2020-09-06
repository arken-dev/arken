CHARON_ENV   = os.getenv("CHARON_ENV") or "test"

local empty  = require('charon.empty')
local test   = require('charon.test')
local notify = require('arken.notify')

local Test = Class.new("routines.Test")

Test.help = {}

local run = function(file)
  local buffer = ""

  package.reload()

  local init      = os.microtime()
  local icon      = "ok"
  local flag, res = pcall(dofile, file)

  if flag == false then
    icon = 'failure'
    return icon, res
  end

  if res == false then
    icon = 'failure'
  end

  if empty(res) then
    icon = 'warning'
  end

  if type(res) == 'table' then
    local footer = ""
    local results = test.run(res)

    local count = {}
    count.ok      = 0
    count.failure = 0
    count.pending = 0

    for description, result in pairs(results) do
      count[result.status] = count[result.status] + 1
      if result.status ~= 'ok' then
        buffer = buffer .. '<b>' .. description .. '</b>' .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          buffer = buffer .. tostring(result.msg) .. '\n\n'
        end
      end

      if count.pending > 0 or count.ok == 0 then
        icon = "warning"
      end

      if count.failure > 0 then
        icon = "failure"
      end

      footer = ""
      for i, v in pairs(count) do
        if footer:len() > 1 then
          footer = footer .. ', '
        end
          footer = footer .. v .. " " .. i
      end
    end
    buffer = buffer .. footer
    --test.output(buffer)
  end

  --if buffer == '' then
  --  buffer = "empty..."
  --end

  local seconds = string.format("%.6f", os.microtime() - init)
  local result  = buffer .. '\n\nFinished in ' .. seconds .. ' seconds'
  --buffer = ""
  return icon, result --:replace('\n', '<br>')
end


-------------------------------------------------------------------------------
-- NOTIFY
-------------------------------------------------------------------------------

Test.help.notify = [[
  test file with result notify
]]

function Test:notify()
  local params = self:params()
  local file   = params[1]
  local ctime  = 0
  while true do
    if os.exists(file) then
      local tmp = os.ctime(file)
      if ctime < tmp then
        ctime = tmp
        local icon, body = run(file)
        notify.send(file, body)      
        print('\n' .. body .. '\n')
      end
    end
    os.sleep(0.25)
  end
end

return Test
