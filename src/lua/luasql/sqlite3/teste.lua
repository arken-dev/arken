local driver = require "luasql.sqlite3"
local env    = driver.sqlite3()
local conn   = env:connect(":memory:")

local sql = [[
  CREATE TABLE IF NOT EXISTS person (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT, total REAL
  )]]

local cursor, message = conn:execute(sql)
if message ~= nil then
  error(message)
end

local names = {'John', 'Steve'}
for _, name in ipairs(names) do
  local sql = string.format("INSERT INTO person (name) VALUES ('%s')", name)
  print(sql)
  local cursor, message = conn:execute(sql)
  if message ~= nil then
    error(message)
  end
  print(conn:getlastautoid())
end

--for i = 1, 10000 do
local cursor, message = conn:execute([[ SELECT name FROM person ]])
--[[
local row = cursor:fetch({}, 'a')
while( row ) do
  local var = 'name'.. row.name
  print(row)
  print(row.name)
  row = cursor:fetch({}, 'a')
end
]]

for row in cursor:each({}) do
  local var = 'name'.. row.name
  print(row)
  print(row.name)
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
