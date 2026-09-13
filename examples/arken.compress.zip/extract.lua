-- arken.compress.zip example: extract
--
-- Zip.extract(archive, output) é uma função estática (chamada em cima do
-- módulo, não de uma instância) — só lê o .zip e grava os arquivos em
-- `output` (criando o diretório se não existir). Devolve true/false.

local Zip = require('arken.compress.Zip')

local archive = os.pwd() .. '/examples/arken.compress.zip/extract-source.zip'
local output  = os.pwd() .. '/examples/arken.compress.zip/extract-output'

os.remove(archive)
os.rmpath(output)

local zip = Zip.new(archive)
zip:addBuffer('hello.txt', 'olá do arken.compress.zip')
zip:addBuffer('user.json', '{"id":42,"name":"Ada"}')
zip:save()

local ok = Zip.extract(archive, output)
print('extract OK? ' .. tostring(ok))

print('hello.txt -> ' .. os.read(output .. '/hello.txt'))
print('user.json -> ' .. os.read(output .. '/user.json'))

os.remove(archive) -- aqui o zip é só um passo intermediário do exemplo, não um artefato
os.rmpath(output)
