local template = require 'arken.template'
local test = {}

test.should_return_function = function()
  template.mtime = {}
  local result = template.execute("util/template/example-equal-char.tpl", {}, {}, true)
  assert( result == 'Hello World !!!', result )
end

test.should_reload_with_flag = function()
  template.cache["util/template/example-equal-char.tpl"] = function(self, helper)
    return "Hello..."
  end
  local result = template.execute("util/template/example-equal-char.tpl", {})
  assert( result == 'Hello...', result )
  template.mtime["util/template/example-equal-char.tpl"] = 0
  local result = template.execute("util/template/example-equal-char.tpl", {}, {}, true)
  assert( result == 'Hello World !!!', result )
end

return test
