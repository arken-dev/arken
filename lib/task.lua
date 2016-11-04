-------------------------------------------------------------------------------
-- PARSE TASK
-------------------------------------------------------------------------------

local task   = arg[1]
local last   = task:lastIndexOf(":")
local name   = task:left(last):replace(":", ".")
local action = task:right(task:len() - last - 1)
if last < 0 then
  name   = arg[1]
  action = nil
end

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

result, module = pcall(require, name:camelcase() .. "Task")
if result == false then
  print(name .. " not work" .. module)
  os.exit()
end

-------------------------------------------------------------------------------
-- EXECUTE
-------------------------------------------------------------------------------
local params = {}
local key = ""

for index, value in ipairs(arg) do
  if value:startsWith("--") then
    key = value:replace("--", "")
    params[key] = parseArgs(index, arg)
  end
end

if module[action] then
  local object = module.new()
  if params.help then
    if object.help[action] then
      local help   = object.help[action]
      local margem = '\n' .. string.rep(' ', #action + 1)
      print(action .. ' # ' .. help:trimmed():swap('\n', margem))
    else
      print(action .. ": undocumented")
    end
  else
    object:execute(action, params)
  end
else
  local help = ""

  if params.help then
    local size = 0
    for k, v in pairs(module.help) do
      if #k > size then
        size = #k
      end
    end

    local margem = '\n' .. string.rep(' ', size + 1)

    for k, v in pairs(module.help) do
      local space = string.rep(' ', size - #k)
      help = help .. k .. space .. ' # ' .. v:trimmed():swap('\n', margem) .. '\n'
    end

  end
  if #help > 0 then
    print(help)
  else
    print(action .. " not include in " .. name)
  end
end
