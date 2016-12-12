-------------------------------------------------------------------------------
-- PATH
-------------------------------------------------------------------------------

package.path  = package.path  .. ";" .. CHARON_PATH .. "/lib/?.lua"
package.path  = package.path  .. ";" .. CHARON_PATH .. "/rhea/?.lua"
package.cpath = package.cpath .. ";" .. CHARON_PATH .. "/clib/?.so"
package.cpath = package.cpath .. ";" .. CHARON_PATH .. "/clib/?.dylib"
package.cpath = package.cpath .. ";" .. CHARON_PATH .. "/clib/?.dll"

-------------------------------------------------------------------------------
-- ENV
-------------------------------------------------------------------------------

CHARON_ENV = os.getenv("CHARON_ENV") or "development"
os.setlocale("C", "numeric")

-------------------------------------------------------------------------------
-- Base
-------------------------------------------------------------------------------
require 'charon.json'
require 'charon.helper'
require 'charon.base.package'
require 'charon.parse'
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

-------------------------------------------------------------------------------
-- OOP
-------------------------------------------------------------------------------

require "Class"
require "Object"

-------------------------------------------------------------------------------
-- PROFILE.D
-------------------------------------------------------------------------------

iterator = QDirIterator.new(CHARON_PATH .. '/profile.d')
while(iterator:hasNext()) do
  iterator:next()
  local fileInfo = iterator:fileInfo()
  if(fileInfo:suffix() == 'lua') then
    dofile(fileInfo:filePath())
  end
end

-------------------------------------------------------------------------------
-- LOCAL PROFILE
-------------------------------------------------------------------------------

if CHARON_PATH ~= QDir.currentPath() then
  local profile = QDir.currentPath() .. '/profile.lua'
  if QFile.exists(profile) then
    dofile(profile)
  end
end
