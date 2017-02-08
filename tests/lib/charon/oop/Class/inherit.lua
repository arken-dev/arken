local Class = require('charon.oop.Class')
local test  = {}

test.redefine_new_method_in_inherit_callback = function()
  local A = Class.new("A")
  A.inherit = function(class)
    class.newOld = class.new
    class.new = function(record)
      record = record or {}
      record.inherit_works = true
      return class.newOld(record)
    end
  end
  local B = Class.new("B", A)
  assert(B.new().inherit_works == true, type(B.new().inherit_works))
end

return test
