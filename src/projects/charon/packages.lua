local HttpClient = require('charon.net.HttpClient')
local zip        = require('charon.compress.zip')
local packages   = string.format("%s/config/packages", CHARON_PATH)


if not os.exists(packages) then
  print(packages .. ' not exists using default')
  packages = string.format("%s/config/packages.default", CHARON_PATH)
end
for package in io.lines(packages) do
  print('package ' .. package)
  package = package:simplified()
  local name     = package:sub(1, package:indexOf(' ')-1)
  local url      = package:sub(package:indexOf(' ')+1)
  local filename = string.format("%s/packages/%s.zip", CHARON_PATH, name)
  local data     = os.read(url)
  local file     = io.open(filename, "w+")
  file:write(data)
  file:close()
  zip.decompress(filename, CHARON_PATH .. '/packages/' .. name)
end
