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

local mvm  = require 'arken.mvm'
local root = os.name() == 'windows' and os.root() or '/etc'
local list = os.glob(string.format('%s/arken/profile.d/*.lua', root))

for fileName in list:each() do
  dofile(fileName)
end

-------------------------------------------------------------------------------
-- LOCAL PROFILE
-------------------------------------------------------------------------------

if mvm.path() ~= os.pwd() then
  local profile = os.pwd() .. '/config/profile.lua'
  if os.exists(profile) then
    dofile(profile)
  end
end
