MANAGER_PATH  = '/opt/objectdata/manager'
empty = require('arken.empty')
package.path = package.path .. ";" .. MANAGER_PATH .. "/app/?.lua"
package.path = package.path .. ";" .. MANAGER_PATH .. "/lib/?.lua"
package.path = package.path .. ";" .. MANAGER_PATH .. "/app/channels/?.lua"

Class = require('arken.oop.Class')
