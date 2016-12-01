local Build = Class.new("Build")

Build.help = {}

Build.help.clear = [[
  clear Makefile, clib dir and shared.
]]

function Build:clear(params)
  print("clear Makefile, .o")
  iterator = QDirIterator.new(OBERON_PATH .. '/src', {"Subdirectories"})
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
  iterator = QDirIterator.new(OBERON_PATH .. '/clib', {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if fileInfo:suffix() == 'so' or fileInfo:suffix() == 'dll' then
      print("remove " .. fileInfo:filePath())
      QFile.remove(fileInfo:filePath())
    end
  end

  print("clear chared")
  iterator = QDirIterator.new(OBERON_PATH .. '/shared', {"Subdirectories"})
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    if fileInfo:fileName():contains('.so') or fileInfo:suffix() == 'dll' then
      print("remove " .. fileInfo:filePath())
      QFile.remove(fileInfo:filePath())
    end
  end

end

return Build
