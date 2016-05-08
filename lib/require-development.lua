package.msecs = QDateTime.currentMSecsSinceEpoch()
package.temp  = {}
package.last  = ""
package.mixed = {}
package.cache = {}

package.requireReload = function(path, mixin)
  if package.loaded[path] then
    if package.isPathUpdated(path) then
      return package.reloadPath(path)
    end
  else
    if mixin then
      package.mixed[package.last] = package.mixed[package.last] or {}
      table.insert(package.mixed[package.last], path)
    else
      package.last = path
    end
  end
  return require(path)
end

package.isPathUpdated = function(path)
  local filename = package.pathToFilename(path)
  if filename then
    if package.isFilenameUpdated(filename) then
      return true
    else
      if type(package.mixed[path]) == 'table' then
        for _, mixin_path in ipairs(package.mixed[path]) do
          local file_mixin = package.pathToFilename(mixin_path)
          if file_mixin then
            if package.isFilenameUpdated(file_mixin) then
              return true
            end
          end
        end
      end
    end
  end
  return false
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
  if not package.isReloaded(path) then
    print('reload: ' .. path)
    local filename = package.pathToFilename(path)
    if filename then
      package.loaded[path] = loadfile(filename)()
    end
     table.insert(package.temp, path)
  end
  return package.loaded[path]
end

package.isReloaded = function(path)
  for _, value in ipairs(package.temp) do
    if value == path then
      return true
    end
  end

  return false
end

package.reload = function()
  package.temp = {}
  for path, table in pairs(package.loaded) do
    if package.cache[path] ~= false then
      package.requireReload(path)
    end
  end
  package.msecs = QDateTime.currentMSecsSinceEpoch()
end
