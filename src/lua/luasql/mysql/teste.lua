local mysql = require "luasql.mysql"
local env  = mysql.mysql()
local conn = env:connect('mysql', 'root', '')
--for i = 1, 10000 do
local cursor, message = conn:execute([[ SELECT Host, User FROM user ]])

for row in cursor:each() do
  local var = 'Host '.. row.Host .. 'User' .. row.User
  print(row)
  print(i)
  print(var)
end

--[[
for i = 1, cursor:size() do
  local row = cursor:at(i)
  local var = 'Host '.. row.Host .. 'User' .. row.User
  print(row)
  print(i)
  print(var)
end
]]
