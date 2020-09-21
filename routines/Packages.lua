local HttpClient = require('charon.net.HttpClient')
local zip        = require('charon.compress.zip')
local packages   = string.format("%s/config/packages", CHARON_PATH)


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
    packages = string.format("%s/config/packages.default", CHARON_PATH)
  end

  for package in io.lines(packages) do
    print('package ' .. package)
    package = package:simplified()
    local name     = package:sub(1, package:indexOf(' ')-1)
    local url      = package:sub(package:indexOf(' ')+1)

    if url:endsWith(".git") then
      print( string.format("%s/packages/%s", CHARON_PATH, name) )
      if os.exists(string.format("%s/packages/%s", CHARON_PATH, name)) then
        os.execute(string.format("cd %s/packages/%s; git pull", CHARON_PATH, name))
      else
        os.execute(string.format("git clone %s %s/packages/%s", url, CHARON_PATH, name))
      end
    else
      local filename = string.format("%s/packages/%s.zip", CHARON_PATH, name)
      local data     = os.read(url)
      local file     = io.open(filename, "w+")
      file:write(data)
      file:close()
      zip.decompress(filename, CHARON_PATH .. '/packages/' .. name)
    end
  end
end

return Packages
