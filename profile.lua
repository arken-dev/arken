-------------------------------------------------------------------------------
-- PATH
-------------------------------------------------------------------------------
ARKEN_PATH = ARKEN_PATH or CHARON_PATH

package.path  = package.path  .. ";" .. ARKEN_PATH .. "/?.lua"
package.path  = package.path  .. ";" .. ARKEN_PATH .. "/lib/?.lua"
package.path  = package.path  .. ";" .. ARKEN_PATH .. "/packages/?.lua"
package.cpath = package.cpath .. ";" .. ARKEN_PATH .. "/clib/?.so"
package.cpath = package.cpath .. ";" .. ARKEN_PATH .. "/clib/?.dylib"
package.cpath = package.cpath .. ";" .. ARKEN_PATH .. "/clib/?.dll"

-------------------------------------------------------------------------------
-- ENV
-------------------------------------------------------------------------------

ARKEN_ENV = os.getenv("ARKEN_ENV") or "development"
os.setlocale("C", "numeric")

-------------------------------------------------------------------------------
-- GLOBALS
-------------------------------------------------------------------------------
require 'arken.base'
require 'arken.package'

-------------------------------------------------------------------------------
-- PROFILE.D
-------------------------------------------------------------------------------

local list = os.glob(ARKEN_PATH .. '/profile.d')
for i = 1, list:size() do
  local fileName = list:at(i)
  if fileName:endsWith(".lua") then
    dofile(fileName)
  end
end

-------------------------------------------------------------------------------
-- LOCAL PROFILE
-------------------------------------------------------------------------------

if ARKEN_PATH ~= os.pwd() then
  local profile = os.pwd() .. '/profile.lua'
  if os.exists(profile) then
    dofile(profile)
  end
end
