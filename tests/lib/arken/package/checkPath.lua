local package = require 'arken.package'
local test    = {}

test.should_return_if_file_is_update = function()
  package.output = function() end
  package.time = 0
  local lib = package.checkPath('util.lib.example')
  assert(  type(lib) == 'table' )
  package.time = os.microtime()
  local lib = package.checkPath('util.lib.example')
  assert(  type(lib) == 'table' )
  package.output = print
end

test.should_return_module_mixed = function()
  package.output = function() end
  local lib = package.checkPath('util.lib.mix')
  package.time = 0
  assert( package.isPathUpdated('util.lib.mix') == true )
  assert(  type(lib) == 'table' )
  package.time = 0 --os.microtime()
  local lib = package.checkPath('util.lib.mix')
  assert(  type(lib) == 'table' )
  package.output = print
end

return test
