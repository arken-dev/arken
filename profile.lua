package.path  = package.path  .. ";" .. OBERON_PATH .. "/lib/?.lua"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.so"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.dll"
package.path  = package.path  .. ";" .. OBERON_PATH .. "/tasks/?.lua"

-------------------------------------------------------------------------------
-- Base
-------------------------------------------------------------------------------

require 'oberon.base'
require 'toboolean'

-------------------------------------------------------------------------------
-- String
-------------------------------------------------------------------------------

require "QString"
require "QByteArrayList"
require "QStringList"
require "QRegExp"

-------------------------------------------------------------------------------
-- QDir and QFile
-------------------------------------------------------------------------------

require "QDir"
require "QDirIterator"
require "QFile"
require "QFileInfo"
require "QFileInfoList"

-------------------------------------------------------------------------------
-- QDate, QTime and QDateTime
-------------------------------------------------------------------------------

require "QDateTime"

-------------------------------------------------------------------------------
-- OOP
-------------------------------------------------------------------------------

require "Class"
require "Object"
