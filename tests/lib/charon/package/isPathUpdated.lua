local package = require 'charon.package'
local test    = {}

test.should_return_if_file_is_update = function()
  package.time = 0
  assert( package.isPathUpdated('util.lib.example') == true )
  package.time = os.microtime()
  assert( package.isPathUpdated('util.lib.example') == false )
end

test.should_return_false_if_file_is_unknow = function()
  package.time = 0
  assert( package.isPathUpdated('util.lib.unknow') == false )
  package.time = os.microtime()
  assert( package.isPathUpdated('util.lib.unknow') == false )
end

return test
