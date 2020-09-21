local list = {'.'} --'app', 'lib', 'tests', 'util', 'importacoes', 'api'}
for _, dir in ipairs(list) do
  local list = os.glob(dir, '\\.cpp$|\\.lua|\\.h|\\.md', true)

  for fileName in list:each() do

    print(fileName)

    local buffer = os.read(fileName)
    local buffer = buffer:replace('arken', 'arken')
    local buffer = buffer:replace('ARKEN', 'ARKEN')
    local buffer = buffer:replace('Arken', 'Arken')

    local file = io.open(fileName, 'w')
    file:write(buffer)
    file:close()
  end
end
