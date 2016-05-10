package.msecs = QDateTime.currentMSecsSinceEpoch()
package.mixed = {}
package.cache = {}

package.requireReload = function(path)
  if package.loaded[path] then
    if package.isPathUpdated(path) then
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

  if package.cache[path] == nil or not QFile.exists(package.cache[path]) then
    for str in string.gmatch(package.path, "([^;]+)") do
      local file_name = tostring(str:gsub("?", path:replace('.', '/')))
      if QFileInfo.exists(file_name) then
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
  local  fileInfo = QFileInfo.new(file_name)
  return fileInfo:lastModified():toMSecsSinceEpoch() > package.msecs
end

package.reloadPath = function(path)
    print('reload: ' .. path)
    local filename = package.pathToFilename(path)
    if filename then
      package.loaded[path] = loadfile(filename)()
    end
  return package.loaded[path]
end

package.reload = function()
  local init = QDateTime.currentMSecsSinceEpoch()
  for path, table in pairs(package.loaded) do
    if package.cache[path] ~= false then
      if package.mixed[path] then
        local class = package.mixed[path]
        print('reload: ' .. path)
        package.loaded[path]  = nil
        package.loaded[class] = nil
        path = class
      end
      package.requireReload(path)
    end
  end
  package.msecs = QDateTime.currentMSecsSinceEpoch()
  return (package.msecs - init) / 1000.0
end
