require "<%= self.class_name -%>"

local should = require "test.should"
local test   = {}

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

return test
