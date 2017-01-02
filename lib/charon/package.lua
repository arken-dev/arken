-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

package.time = os.microtime()
package.mixed = {}
package.cache = {}

package.checkPath= function(path)
  if package.loaded[path] then
    if package.isPathUpdated(path) then
      if package.mixed[path] then
        local class = package.mixed[path]
        print('reload: ', path)
        package.loaded[path]  = nil
        package.loaded[class] = nil
        path = class
      end
      return package.reloadPath(path)
    end
  end
  return require(path)
end

package.isPathUpdated = function(path)
  local filename = package.pathToFilename(path)
  if filename then
    return package.isFilenameUpdated(filename)
  else
    return false
  end
end

package.pathToFilename = function(path)
  if package.cache[path] == nil or not os.exists(package.cache[path]) then
    for str in string.gmatch(package.path, "([^;]+)") do
      local file_name = tostring(str:gsub("?", path:replaceChars('.', '/')))
      if os.exists(file_name) then
        package.cache[path] = file_name
        break
      end
    end
  end

  if package.cache[path] == nil then
    package.cache[path] = false
  end

  return package.cache[path]
end

package.isFilenameUpdated = function(file_name)
  return os.ctime(file_name) > package.time
end

package.reloadPath = function(path)
    print('reload: ', path)
    local filename = package.pathToFilename(path)
    if filename then
      package.loaded[path] = assert(dofile(filename))
    end
  return package.loaded[path]
end

package.reload = function()
  local init = os.microtime()
  for path, table in pairs(package.loaded) do
    if package.cache[path] ~= false then
      package.checkPath(path)
    end
  end
  package.time = os.microtime()
  return (package.time - init)
end

return package
