package.path  = package.path  .. ";" .. OBERON_PATH .. "/lib/?.lua"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.so"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.dll"
package.path  = package.path  .. ";" .. OBERON_PATH .. "/tasks/?.lua"

require 'oberon.base'
