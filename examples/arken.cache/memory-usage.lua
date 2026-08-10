-- arken.cache example: diagnóstico de memória por bucket
--
-- Cenário: a memória do processo está alta e você precisa descobrir se a
-- culpa é do cache — e, se for, qual bucket está pesando. cache.buckets()
-- lista todo bucket já usado no processo; bucket.size() devolve os bytes
-- ocupados pelos VALORES daquele bucket (não conta chaves nem overhead de
-- estrutura, então é uma cota-parte, não o RSS do processo).

local cache = require('arken.cache')

local session = cache.bucket('sessao')
local html    = cache.bucket('html')
local upload  = cache.bucket('upload')

session.insert('user:1', { id = 1, nome = 'Ada' })
html.put('home:fragment', string.rep('<div>fragmento</div>', 50))
upload.put('tmp:relatorio.pdf', string.rep('x', 200000))

-- formata bytes em unidade legível
local function humanSize(bytes)
  local units = { 'B', 'KB', 'MB', 'GB' }
  local i = 1
  while bytes >= 1024 and i < #units do
    bytes = bytes / 1024
    i = i + 1
  end
  return string.format('%.2f %s', bytes, units[i])
end

-- itera todos os buckets em uso e soma o tamanho de cada um
local report = {}
local total  = 0

for _, name in ipairs(cache.buckets()) do
  local size = cache.bucket(name).size()
  table.insert(report, { name = name, bytes = size })
  total = total + size
end

-- ordena do maior para o menor, pra achar o principal suspeito primeiro
table.sort(report, function(a, b) return a.bytes > b.bytes end)

print('-- uso de memória por bucket --')
for _, entry in ipairs(report) do
  print(string.format('  %-10s %s', entry.name, humanSize(entry.bytes)))
end
print('-- total --')
print('  ' .. humanSize(total))

-- limiar de alerta: qualquer bucket sozinho acima de 100KB (ajuste ao caso
-- real) entra numa lista de suspeitos pra investigar primeiro
local threshold = 100 * 1024
print('-- suspeitos (> ' .. humanSize(threshold) .. ') --')
for _, entry in ipairs(report) do
  if entry.bytes > threshold then
    print('  ' .. entry.name .. ': ' .. humanSize(entry.bytes)
      .. ' (' .. #cache.bucket(entry.name).keys() .. ' chaves)')
  end
end

session.remove('user:1')
html.remove('home:fragment')
upload.remove('tmp:relatorio.pdf')
