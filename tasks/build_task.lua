local BuildTask = Class.new("BuildTask")

function BuildTask:clear(params)
  print("clear Makefile")
  iterator = QDirIterator.new(OBERON_PATH .. '/src')
  while(iterator:hasNext()) do
    iterator:next()
    local fileInfo = iterator:fileInfo()
    print(fileInfo:fileName())
  end

end
