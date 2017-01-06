local mysql = require "luasql.mysql"
local env  = mysql.mysql()
local conn = env:connect('mysql', 'root', '')
local cursor, message = conn:execute([[ SELECT Host, User FROM user ]])
local row = cursor:fetch ({}, "a")
while row do
  print( row.Host )
  print( row.User )
  row = cursor:fetch ({}, "a")
end

