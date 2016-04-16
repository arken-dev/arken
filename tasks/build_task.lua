local BuildTask = Class.new("BuildTask")

function BuildTask:clear(params)
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
end

return BuildTask
