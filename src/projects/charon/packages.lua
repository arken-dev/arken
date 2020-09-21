local HttpClient = require('arken.net.HttpClient')
local zip        = require('arken.compress.zip')
local packages   = string.format("%s/config/packages", ARKEN_PATH)


if not os.exists(packages) then
  print(packages .. ' not exists using default')
  packages = string.format("%s/config/packages.default", ARKEN_PATH)
end

for package in io.lines(packages) do
  print('package ' .. package)
  package = package:simplified()
  local name     = package:sub(1, package:indexOf(' ')-1)
  local url      = package:sub(package:indexOf(' ')+1)

  if url:endsWith(".git") then
    print( string.format("%s/packages/%s", ARKEN_PATH, name) )
    if os.exists(string.format("%s/packages/%s", ARKEN_PATH, name)) then
      os.execute(string.format("cd %s/packages/%s; git pull", ARKEN_PATH, name))
    else
      os.execute(string.format("git clone %s %s/packages/%s", url, ARKEN_PATH, name))
    end
  else
    local filename = string.format("%s/packages/%s.zip", ARKEN_PATH, name)
    local data     = os.read(url)
    local file     = io.open(filename, "w+")
    file:write(data)
    file:close()
    zip.decompress(filename, ARKEN_PATH .. '/packages/' .. name)
  end
end
