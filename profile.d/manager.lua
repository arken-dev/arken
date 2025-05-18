-------------------------------------------------------------------------------
-- MANAGER PATH
-------------------------------------------------------------------------------

MANAGER_PATH  = '/opt/objectdata/manager'

package.path = package.path .. ";" .. MANAGER_PATH .. "/app/?.lua"
package.path = package.path .. ";" .. MANAGER_PATH .. "/lib/?.lua"
package.path = package.path .. ";" .. MANAGER_PATH .. "/app/channels/?.lua"
package.path = package.path .. ";" .. MANAGER_PATH .. "/?.lua"
package.path = package.path .. ";app/clients/?.lua"
package.path = package.path .. ";app/models/?.lua"
package.path = package.path .. ";app/controllers/?.lua"
package.path = package.path .. ";app/helpers/?.lua"
package.path = package.path .. ";lib/?.lua"

-------------------------------------------------------------------------------
-- GLOBALS
-------------------------------------------------------------------------------
Class     = require('arken.oop.Class')
toboolean = require('arken.toboolean')
JSON      = require('arken.jsonp')
JSON.null = JSON.decode("null")
empty     = require('arken.empty')
Date      = require('arken.chrono.Date')
Time      = require('arken.chrono.Time')
os.iostat = require('lib.iostat')
os.iowait = function()
  return os.iostat().iowait
end

os.exec = function(cmd, sanitize)
  --local handle = assert(io.popen(cmd, 'r'))
  --local output = assert(handle:read('*a'))
  --handle:close()
  local tmpname = os.tmpname()
  os.execute(cmd .. " > " .. tmpname)
  local output = os.read(tmpname)
  os.remove(tmpname)
  if sanitize then
    output = string.gsub(output, '^%s+', '')
    output = string.gsub(output, '%s+$', '')
    output = string.gsub(output, '[\n\r]+', ' ')
  end
  return output
end

os.cpu = function()
  local buffer = os.exec('cat /proc/cpuinfo')
  local lines  = buffer:split('\n')
  local name   = nil
  for line in lines:each() do
    if line:contains('model name') then
      name = line:suffix(': ')
    end
  end
  return name
end

-------------------------------------------------------------------------------
-- TABLE#SHUFFLE
-------------------------------------------------------------------------------

table.shuffle = function(t)
  math.randomseed(os.microtime())
  local result = {}
  for i = 1, #t do
    result[i] = t[i]
  end
  for i = #result, 2, -1 do
    local j = math.random(i)
    result[i], result[j] = result[j], result[i]
  end
  return result
end

-------------------------------------------------------------------------------
-- ROUTINE
-------------------------------------------------------------------------------

local routine = require('arken.routine')
  routine._run = routine.run
  routine.run = function(arg)
  local params = {}
  for i = 1, #arg do
    params[i] = arg[i]
  end
  local name = "/opt/objectdata/manager/log/routine.log"
  local Log  = require('arken.Log')
  local log  = Log.new(name)
  log:info(arg[0] .. " " .. table.concat(params, " "))
  log:dump()
  routine._run(arg)
end

-------------------------------------------------------------------------------
-- MANAGER SETUP
-------------------------------------------------------------------------------

local cron = false
local list = os.find("/etc/cron.d/", "base-.*$", true)
for fileName in list:each() do
  cron = true
end
if cron == false and (os.getenv('USER') ~= 'root' or arg[0] ~= 'manager:setup') then
  print('arquivo /etc/cron.d/base não existe, execute arken manager:setup como root')
  os.exit()
end
