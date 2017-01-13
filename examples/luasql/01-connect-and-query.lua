local driver = require "luasql.postgres"
local env    = driver.postgres()
local conn   = env:connect('fusion_development', 'postgres', '', 'localhost')
local cursor, message = conn:execute([[ SELECT attname FROM pg_attribute ]])
local row = cursor:fetch ({}, "a")
while row do
  local attnanem = row.attname
  --print( row.attname )
  row = cursor:fetch ({}, "a")
end

