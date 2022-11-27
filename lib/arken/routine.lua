-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.
local Array = require('arken.Array')

local routine = {}

routine.output = print

routine.parseArg = function(index, args)
  index = index + 1
  local result = Array.new()
  while (index <= #args) do
    if args[index]:startsWith("--") then
      break
    end
    table.insert(result, args[index])
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

routine.parseArgs = function(args)
  local params = {}
  local key = ""

  for index, value in ipairs(args) do
    if value:startsWith("--") then
      key = value:mid(3, -1)
      params[key] = routine.parseArg(index, args)
    else
      table.insert(params, value)
    end
  end

  return params
end

routine.parseName = function(arg)
  local lastIndexMethod = arg:lastIndexOf(":")
  local lastIndexName   = arg:lastIndexOf(".")
  if lastIndexName < 0 then
    lastIndexName = 0
  end
  local space  = arg:mid(1, lastIndexName-1)
  local name   = arg:sub(lastIndexName+1, lastIndexMethod-1)
  local method = arg:sub(lastIndexMethod+1)
  if lastIndexName > 0 then
    space = space .. "."
  else
    space = ""
  end
  return (space .. name:camelCase()), method
end

routine.help = function(module)
  local help = ""
  local size = 0

  if module.help then

    for k, v in pairs(module.help) do
      if #k > size then
        size = #k
      end
    end

    local margem = '\n' .. string.rep(' ', size + 1)

    for k, v in pairs(module.help) do
      local space = string.rep(' ', size - #k)
      help = help .. k .. space .. ' # ' .. v:trim():replace('\n', margem) .. '\n'
    end

    routine.output(help)
  end
end

routine.run = function(args)
  local path = {}

  if args[0] == nil then
    local list = tostring(package.path):split(';')
    for i = 1, list:size() do
      local str = list:at(i)
      str = str:replace("/?.lua", ""):replace('.', '/')
      str = os.abspath(str)
      str = str .. '/routines'
      if os.exists(str) then
        table.insert(path, str)
      end
    end
    for _, str in ipairs(path) do
      local list = os.find(str, "\\.lua$", false)
      for i = 1, list:size() do
        local module = dofile(list:at(i))
        routine.output(module.className:underscore())
        routine.help(module)
      end
    end
  else
    local name, action = routine.parseName(args[0])
    local module = require('routines.' .. name)

    if action == 'help' then
      routine.output(module.className:underscore())
      routine.help(module)
      os.exit()
    end

    -------------------------------------------------------------------------------
    -- EXECUTE
    -------------------------------------------------------------------------------
    local params = routine.parseArgs(args)

    if module[action] then
      local object = module.new(params)
      if params.help then
        if object.help[action] then
          local help   = object.help[action]
          local margem = '\n' .. string.rep(' ', #action + 1)
          routine.output(action .. ' # ' .. help:trim():replace('\n', margem))
        else
          routine.output(action .. ": undocumented")
        end
      else
        object:execute(action, params)
      end
    else
      if params.help then
        routine.help(module)
      end
    end
  end
end

-- execute routine passing all args
-- routine.execute('routine:action', 'param1', 'param2')
-- example: routine.execute('migrate:up')
routine.execute = function(...)
  local tmp = {}
  for i, value in ipairs({...}) do
    tmp[i-1] = value
  end
  routine.run(tmp)
end

-- execute routine with forward args params
-- rotine.foward('routine:action') args passing console send params
routine.forward = function(name)
  local tmp = {}
  for i, value in ipairs(arg) do
    tmp[i] = value
  end
  tmp[0] = name
  routine.run(tmp)
end

return routine
