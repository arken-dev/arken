package.path  = package.path  .. ";" .. OBERON_PATH .. "/lib/?.lua"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.so"
package.cpath = package.cpath .. ";" .. OBERON_PATH .. "/clib/?.dll"
package.path  = package.path  .. ";" .. OBERON_PATH .. "/tasks/?.lua"

-------------------------------------------------------------------------------
-- Base
-------------------------------------------------------------------------------

require 'oberon.helper'
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
