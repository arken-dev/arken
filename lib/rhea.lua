-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local ByteArrayList = require "charon.ByteArrayList"
local rhea = {}

rhea.output = print

rhea.parseArg = function(index, args)
  index = index + 1
  local result = {}
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

rhea.parseArgs = function(args)
  local params = {}
  local key = ""

  for index, value in ipairs(args) do
    if value:startsWith("--") then
      key = value:mid(3, -1)
      params[key] = rhea.parseArg(index, args)
    end
  end

  return params
end

rhea.help = function(module)
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
      help = help .. k .. space .. ' # ' .. v:trim():replaceAll('\n', margem) .. '\n'
    end

    rhea.output(help)
  end
end

rhea.run = function(args)
  local path = {}

  if args[0] == nil then
  local list = tostring(package.path):split(';')
  for i = 1, list:size() do
    local str = list:at(i)
    str = str:replaceAll("/?.lua", ""):replaceChars('.', '/')
    str = os.abspath(str)
    str = str .. '/rhea'
    if os.exists(str) then
      table.insert(path, str)
    end
  end
    for _, str in ipairs(path) do
      local list = os.glob(str, "\\.lua$", false)
      for i = 1, list:size() do
        local module = dofile(list:at(i))
        rhea.output(module.className:underscore())
        rhea.help(module)
      end
    end
  else
    local task   = args[0]
    local last   = task:lastIndexOf(":")
    local name   = args[0]
    local action = nil

    if last > 0 then
      name   = task:left(last-1):replaceChars(":", ".")
      action = task:mid(last+1, -1)
    end

    local module, result
      local rhea_name = 'rhea.' .. name:camelcase()
      local module = require(rhea_name)

    -------------------------------------------------------------------------------
    -- EXECUTE
    -------------------------------------------------------------------------------
    local params = rhea.parseArgs(args)

    if module[action] then
      local object = module.new()
      if params.help then
        if object.help[action] then
          local help   = object.help[action]
          local margem = '\n' .. string.rep(' ', #action + 1)
          rhea.output(action .. ' # ' .. help:trim():replaceAll('\n', margem))
        else
          rhea.output(action .. ": undocumented")
        end
      else
        object:execute(action, params)
      end
    else
      if params.help then
        rhea.help(module)
      end
    end
  end
end

return rhea
