local template = require 'charon.template'
local test = {}

test.should_error_if_file_not_exists = function()
  local status, result = pcall(template.compile, "util/template/file-not-exists.tpl", {})
  assert( status == false )
  assert( result:contains('util/template/file-not-exists.tpl not exists') == true, result )
end

test.should_compile_params_as_local_vars = function()
  local result = template.compile("util/template/example1.tpl", { request = {}, helper = {}, form = {} } )
  assert( result == os.read("util/template/example1.compiled"), result )
end

test.should_compile_equal_char_to_concat = function()
  local result = template.compile("util/template/example-equal-char.tpl", {} )
  assert( result == os.read("util/template/example-equal-char.compiled"), result )
end

test.should_compile_without_equal = function()
  local result = template.compile("util/template/example-without-equal.tpl", {} )
  assert( result == os.read("util/template/example-without-equal.compiled"), result )
end

test.should_compile_without_end_line = function()
  local result = template.compile("util/template/example-without-endline.tpl", {} )
  assert( result == os.read("util/template/example-without-endline.compiled"), result )
end

test.should_compile_multiples_tags = function()
  local result = template.compile("util/template/example-multiples-tags.tpl", {} )
  assert( result == os.read("util/template/example-multiples-tags.compiled"), result )
end

return test
