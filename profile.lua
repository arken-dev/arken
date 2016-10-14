-------------------------------------------------------------------------------
-- PATH
-------------------------------------------------------------------------------

package.path  = package.path  .. ";" .. OBERON_PATH .. "/lib/?.lua"
package.path  = package.path  .. ";" .. OBERON_PATH .. "/tasks/?.lua"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.so"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.dll"

-------------------------------------------------------------------------------
-- ENV
-------------------------------------------------------------------------------

OBERON_ENV = os.getenv("OBERON_ENV") or "development"

-------------------------------------------------------------------------------
-- Base
-------------------------------------------------------------------------------

require 'oberon.helper'
require 'oberon.base.package'
require 'oberon.parse'
require 'toboolean' -- deprecate ?
require 'isblank'   -- deprecate ?
require 'inspect'   -- deprecate ?

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

if OBERON_ENV == 'test' then
  QDateTime.currentDateTimeOld = QDateTime.currentDateTime
  QDateTime.currentDateTime = function()
    local format = "yyyy/MM/dd hh:mm:ss."
    local clock  = tostring(os.clock())
    return QDateTime.currentDateTimeOld():toString(format) .. clock:mid(2, #clock)
  end
end

-------------------------------------------------------------------------------
-- OOP
-------------------------------------------------------------------------------

require "Class"
require "Object"

-------------------------------------------------------------------------------
-- PROFILE.D
-------------------------------------------------------------------------------

iterator = QDirIterator.new(OBERON_PATH .. '/profile.d')
while(iterator:hasNext()) do
  iterator:next()
  local fileInfo = iterator:fileInfo()
  if(fileInfo:suffix() == 'lua') then
    loadfile(fileInfo:filePath())()
  end
end

-------------------------------------------------------------------------------
-- LOCAL PROFILE
-------------------------------------------------------------------------------

if OBERON_PATH ~= QDir.currentPath() then
  local profile = QDir.currentPath() .. '/profile.lua'
  if QFile.exists(profile) then
    print(loadfile(profile)())
  end
end
