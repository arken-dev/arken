local notify = require('arken.notify')
local File   = Class.new("routines.File")
local buffer = ""

_G.print = function(message)
  buffer = buffer .. message .. '\n'
end

File.help = {}

-------------------------------------------------------------------------------
-- NOTIFY
-------------------------------------------------------------------------------

File.help.notify = [[
  run file with notify result
]]

function File:notify()
  local file = arg[1]

  if not file:endsWith('.lua') then
    notify.send('invalid file', 'It is not a .lua file', 'error')  
    os.exit()
  end

  if not os.exists(file) then
    notify.send('invalid file', 'file does not exist', 'error')  
    os.exit()
  end

  local ctime = 0
  while true do
    local tmp = os.ctime(file) 

    if tmp > ctime then
      ctime = tmp

      local status, result = pcall(dofile, file)
      local icon = 'ok'
      if not status then
        icon   = 'error' 
        buffer = result
      end

      notify.send(file, buffer, icon)

      -- clear buffer output captured
      -- for print function rewrite
      buffer = ""

    end
    -- wait 0.05 secs
    os.sleep(0.05)
  end

end

return File
