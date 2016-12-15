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
-- GLOBALS
-------------------------------------------------------------------------------

require 'charon.json'
require 'charon.helper'
require 'charon.package'
require 'charon.parse'
require "CStringList"

-------------------------------------------------------------------------------
-- PROFILE.D
-------------------------------------------------------------------------------

local list = os.glob(CHARON_PATH .. '/profile.d')
for i = 1, list:size() do
  local fileName = list:at(i)
  if fileName:endsWith(".lua") then
    dofile(fileName)
  end
end

-------------------------------------------------------------------------------
-- LOCAL PROFILE
-------------------------------------------------------------------------------

if CHARON_PATH ~= os.pwd() then
  local profile = os.pwd() .. '/profile.lua'
  if os.exists(profile) then
    dofile(profile)
  end
end
