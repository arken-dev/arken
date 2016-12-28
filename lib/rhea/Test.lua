local Class = require 'charon.oop.Class'
local Test  = Class.new("Test")

-------------------------------------------------------------------------------
-- HELP
-------------------------------------------------------------------------------
Test.help = {}

-------------------------------------------------------------------------------
-- SPLIT
-------------------------------------------------------------------------------

Test.help.extract = [[
  receive file name para and extrac all functions methods e create equivalente
  file tests
]]

function Test:extract()
  local fileName = arg[1]
  local module   = dofile(arg[1])
  local filePath = 'tests/' .. fileName:mid(1, fileName:lastIndexOf(".")-1)
  print(fileName)
  print(filePath)
  os.mkpath(filePath)

  if type(module) == 'table' then
    for k, v in pairs(module) do
      if type(v) == 'function' then
        local fileTest = filePath .. '/' .. k .. '.lua'
        if os.exists(fileTest) then
          print( fileTest .. ' exists' )
        else 
          local file = io.open(fileTest, 'w')
          file:write("local test = {}\nreturn test")
          file:close()
          print( fileTest .. ' create' )
        end
      end
    end
  end

  if type(module) == 'function' then
    local fileTest = filePath .. '/tests.lua'
    if os.exists(fileTest) then
      print( fileTest .. ' exists' )
    else 
      local file = io.open(fileTest, 'w')
      file:write("local test = {}\nreturn test")
      file:close()
      print( fileTest .. ' create' )
    end
  end
end

return Test
