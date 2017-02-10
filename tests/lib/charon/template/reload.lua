local template = require 'charon.template'

local test = {}

test.should_return_function = function()
  template.mtime["util/template/example-reload.tpl"] = 0
  local result1 = template.execute("util/template/example-reload.tpl", {var1 = true}, true)
  assert( result1 == 'true - nil', result1 )
  local result2 = template.execute("util/template/example-reload.tpl", {var2 = true}, false)
  assert( result2 == 'nil - nil', result2 )
  local result2 = template.execute("util/template/example-reload.tpl", {var2 = true}, true)
  assert( result2 == 'nil - nil', result2 )
  template.mtime["util/template/example-reload.tpl"] = 0
  local result3 = template.execute("util/template/example-reload.tpl", {var2 = true}, true)
  assert( result3 == 'nil - true', result3 )
end

return test
