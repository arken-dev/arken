local package = require 'charon.package'
local test    = {}

test.should_reload_after_time_module_is_updatetodate = function()
  package.output = function() end
  package.time = os.microtime()
  local lib  = require('util.lib.example')
  local time = lib.localtime()
  package.reload()
  assert( time == lib.localtime() )
  assert( package.isPathUpdated(os.abspath('util/lib/example.lua')) == false )
  package.time = 0
  package.loaded['util.lib.example'] = nil
  package.reload()
  local package = require 'charon.package'
  local lib = require('util.lib.example')
  assert(lib.localtime() > time, string.format("%s %s", lib.localtime(), time) )
  package.output = print
end

return test
