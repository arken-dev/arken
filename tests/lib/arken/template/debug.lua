local template = require 'arken.template'
local test = {}

test.should_source_with_line_number = function()
  template.execute("util/template/example1.tpl", {})
  local result = template.debug("util/template/example1.tpl", {})
  local value  = os.read("util/template/example1.debug")
  assert( result == value, value)
end

test.should_return_error_if_file_not_exists = function()
  local result, message = pcall(template.debug, "util/template/file-not-exists.tpl", {})
  assert( result == false )
  assert( message:contains('file util/template/file-not-exists.tpl not exists') == true, message )
end

return test
