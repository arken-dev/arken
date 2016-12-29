local Build = Class.new("Build")

Build.help = {}

-------------------------------------------------------------------------------
-- CLEAR
-------------------------------------------------------------------------------

Build.help.clear = [[
  clear Makefile, clib dir and shared.
]]

function Build:clear(params)
  print("clear Makefile, .o")
  local iterator = QDirIterator.new(GANYMED_PATH .. '/src', {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    -- .o
    if(fileInfo:suffix() == 'o') then
      print("remove " .. fileInfo:filePath())
      QFile.remove(fileInfo:filePath())
    end
    -- makefile
    if fileInfo:fileName():startsWith("Makefile") then
      print("remove " .. fileInfo:filePath())
      QFile.remove(fileInfo:filePath())
    end
  end

  print("clear clib")
  iterator = QDirIterator.new(GANYMED_PATH .. '/clib', {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if fileInfo:suffix() == 'so' or fileInfo:suffix() == 'dll' then
      print("remove " .. fileInfo:filePath())
      QFile.remove(fileInfo:filePath())
    end
  end

  print("clear chared")
  iterator = QDirIterator.new(GANYMED_PATH .. '/shared', {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if fileInfo:fileName():contains('.so') or fileInfo:suffix() == 'dll' then
      print("remove " .. fileInfo:filePath())
      QFile.remove(fileInfo:filePath())
    end
  end

end

-------------------------------------------------------------------------------
-- LICENSE
-------------------------------------------------------------------------------

Build.help.license = [[
insert header c and cpp files resume of license
]]

function Build:license()
  local dirs = { 'bindings', 'ganymed', 'projects' }
  local license = os.read(GANYMED_PATH .. '/rhea/build/license.header') .. '\n'
  for _, dir in ipairs(dirs) do
    local iterator = QDirIterator.new(GANYMED_PATH .. '/src/' .. dir, {"Subdirectories"})
    while(iterator:hasNext()) do
      iterator:next()
      local fileInfo = iterator:fileInfo()
      if(fileInfo:suffix() == 'c' or fileInfo:suffix() == 'cpp' ) then
        local buffer = os.read(fileInfo:filePath())
        if not buffer:startsWith(license) then
          print(fileInfo:filePath())
          local file = QFile.new(fileInfo:filePath())
          file:open({"WriteOnly"})
          file:write(license .. buffer)
          file:close()
        end
      end
    end
  end
end

-------------------------------------------------------------------------------
-- LICENSE
-------------------------------------------------------------------------------

Build.help.charon = [[
change charon => charon patterns
]]

function Build:charon()
  local iterator = QDirIterator.new(CHARON_PATH, {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if(fileInfo:suffix() == 'c' or fileInfo:suffix() == 'cpp' or fileInfo:suffix() == 'pro' or fileInfo:suffix() == 'lua' ) then
      local buffer = os.read(fileInfo:filePath())
      buffer = buffer:replaceAll("charon", "charon")
      buffer = buffer:replaceAll("Charon", "Charon")
      buffer = buffer:replaceAll("CHARON", "CHARON")
      print(fileInfo:filePath())
      local file = QFile.new(fileInfo:filePath())
      file:open({"WriteOnly"})
      file:write(buffer)
      file:close()
    end
  end
end

return Build
