local Converter = require('charon.Converter')
local test = {}

test.should_merge_other_table = function()
  local con = Converter.new({ description = "%a%" }):merge{ category_id = 15 }
  assert( con.description == '%a%' ) 
  assert( con.category_id == 15 ) 
end

return test
