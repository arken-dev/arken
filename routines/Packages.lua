local HttpClient = require('arken.net.HttpClient')
local zip        = require('arken.compress.zip')
local mvm        = require('arken.mvm')
local packages   = string.format("%s/config/packages", mvm.path())

local Packages = Class.new("routines.Packages")

Packages.help   = {}

-------------------------------------------------------------------------------
-- UPDATE
-------------------------------------------------------------------------------

Packages.help.update = [[
  update all packages
]]

function Packages:update()
  if not os.exists(packages) then
    print(packages .. ' not exists using default')
    packages = string.format("%s/config/packages.default", mvm.path())
  end

  for package in io.lines(packages) do
    print('package ' .. package)
    package = package:simplified()
    local name     = package:sub(1, package:indexOf(' ')-1)
    local url      = package:sub(package:indexOf(' ')+1)

    if url:endsWith(".git") then
      print( string.format("%s/packages/%s", mvm.path(), name) )
      if os.exists(string.format("%s/packages/%s", mvm.path(), name)) then
        os.execute(string.format("cd %s/packages/%s; git pull", mvm.path(), name))
      else
        os.execute(string.format("git clone %s %s/packages/%s", url, mvm.path(), name))
      end
    else
      local filename = string.format("%s/packages/%s.zip", mvm.path(), name)
      local data     = os.read(url)
      local file     = io.open(filename, "w+")
      file:write(data)
      file:close()
      zip.decompress(filename, mvm.path() .. '/packages/' .. name)
    end
  end
end

return Packages
