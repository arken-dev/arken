local Class = require('charon.oop.Class')
local App   = Class.new("App")
App.help    = {}
App.output  = print

-------------------------------------------------------------------------------
-- CREATE
-------------------------------------------------------------------------------

App.help.create = [[
  create app in dir
]]

function App:create(params)
  local dirName = params[1]
  if os.exists(dirName) then
    error(dirName .. ' exists')
  else
    os.copy(CHARON_PATH .. '/skel', dirName)
    for fileName in os.glob(dirName, true):each() do
      if not fileName:endsWith('.') and not fileName:endsWith('..') then
        App.output(fileName)
      end
    end
  end
end

return App
