-- arken.compress.zip example: addBuffer
--
-- addBuffer(name, buffer) escreve o conteúdo direto no zip, sem precisar
-- de um arquivo no disco antes — útil pra gerar entradas a partir de dado
-- que já está em memória (JSON, HTML renderizado, etc).

local Zip = require('arken.compress.Zip')

local output = os.pwd() .. '/examples/arken.compress.zip/add-buffer.zip'
os.remove(output) -- Zip.new abre com ZIP_CREATE: se o arquivo já existir, soma nas entradas antigas

local zip = Zip.new(output)

zip:addBuffer('hello.txt', 'olá do arken.compress.zip')
zip:addBuffer('user.json', '{"id":42,"name":"Ada"}')

zip:save() -- só depois do save() o .zip fica de fato gravado e válido

print('arken.compress.zip addBuffer OK -> ' .. output)
