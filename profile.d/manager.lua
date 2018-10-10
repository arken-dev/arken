MANAGER_PATH  = '/opt/objectdata/manager'
empty = require('charon.empty')
package.path = package.path .. ";" .. MANAGER_PATH .. "/app/?.lua"

Class = require('charon.oop.Class')
