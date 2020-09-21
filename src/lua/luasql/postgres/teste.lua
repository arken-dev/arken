local json   = require "arken.jsonp"
local driver = require "luasql.postgres"
local env  = driver.postgres()
local conn, msg = env:connect('fusion_development', 'postgres', 'abc123', 'localhost')
--print(msg)
--for i = 1, 10000 do
local cursor, message = conn:execute([[ SELECT nome_conhecido FROM usuario ]])
--[[
for row in cursor:each() do
  local var = 'attname '.. row.nome_conhecido
  --print(row)
  --print(i)
  print(row)
  print(var)
end
]]

for i = 1, cursor:size() do
  local row = cursor:at(i)
  local var = 'attname '.. row.nome_conhecido
  --print(row)
  --print(i)
  print(row)
  print(var)
end
