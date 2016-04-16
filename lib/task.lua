-------------------------------------------------------------------------------
-- PARSE TASK
-------------------------------------------------------------------------------

local task   = arg[1]
local last   = task:lastIndexOf(":")
local name   = task:left(last):replace(":", ".")
local action = task:right(task:len() - last - 1)

-------------------------------------------------------------------------------
-- PARSE ARGS
-------------------------------------------------------------------------------

function parseArgs(index, arg)
  index = index + 1
  local result = {}
  while (index <= #arg) do
    if arg[index]:startsWith("--") then
      break
    end
    table.insert(result, arg[index])
    index = index + 1
  end

  if #result == 0 then
    return true
  end

  if #result == 1 then
    return result[1]
  end

  return result
end

-------------------------------------------------------------------------------
-- REQUIRE
-------------------------------------------------------------------------------

result, module = pcall(require, name .. "_task")
if result == false then
  print(name .. " not work" .. module)
  os.exit()
end

-------------------------------------------------------------------------------
-- EXECUTE
-------------------------------------------------------------------------------

if module[action] then
  local params = {}
  local key = ""
  for index, value in ipairs(arg) do
    if value:startsWith("--") then
      key = value:replace("--", "")
      params[key] = parseArgs(index, arg)
    end
  end
  local object = module.new()
  object:execute(action, params)
else
  print(action .. " not include in " .. name)
end
