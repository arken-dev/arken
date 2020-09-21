local list = {'.'} --'app', 'lib', 'tests', 'util', 'importacoes', 'api'}
for _, dir in ipairs(list) do
  local list = os.glob(dir, ".md$", true)

  for fileName in list:each() do

    print(fileName)

    local buffer = os.read(fileName)
    local buffer = buffer:replace('charon', 'arken')
    local buffer = buffer:replace('CHARON', 'ARKEN')

    local file = io.open(fileName, 'w')
    file:write(buffer)
    file:close()
  end
end
