local Class = require('arken.oop.Class')
local M = Class.new("UtilExample")
M.time = os.clock()

M.localtime = function()
   return M.time 
end

return M
