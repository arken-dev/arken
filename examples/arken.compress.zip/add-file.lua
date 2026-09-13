-- arken.compress.zip example: addFile
--
-- addFile(path) lê o arquivo do disco e usa o basename como nome da
-- entrada no zip (ver arken::compress::Zip::addFile) — diferente de
-- addBuffer, aqui você não escolhe o nome interno, ele vem do caminho.

local Zip = require('arken.compress.Zip')

local srcDir = os.pwd() .. '/examples/arken.compress.zip/tmp-src'
os.mkpath(srcDir)

local fileA = srcDir .. '/a.txt'
local fileB = srcDir .. '/b.txt'

local fh = io.open(fileA, 'w'); fh:write('conteúdo de a.txt'); fh:close()
fh      = io.open(fileB, 'w'); fh:write('conteúdo de b.txt'); fh:close()

local output = os.pwd() .. '/examples/arken.compress.zip/add-file.zip'
os.remove(output)

local zip = Zip.new(output)
zip:addFile(fileA) -- vira "a.txt" dentro do zip
zip:addFile(fileB) -- vira "b.txt" dentro do zip
zip:save()

print('arken.compress.zip addFile OK -> ' .. output)

os.rmpath(srcDir) -- só os fontes temporários; o .zip gerado fica como artefato do exemplo
