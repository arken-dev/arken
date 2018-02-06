MANAGER_PATH  = '/opt/objectdata/manager'
package.path = package.path .. ";" .. MANAGER_PATH .. "/app/?.lua"

Class = require('charon.oop.Class')
