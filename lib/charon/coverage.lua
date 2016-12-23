-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

require 'charon.odebug'

local M = {}

local result = {}

local hook = function()
  -- disabling because it is slow
  --[[
  local info = debug.getinfo(2, "Sl")
  local filename     = info.source
  local linedefined  = info.currentline
  ]]

  -- odebug is a charon module return values
  -- whitout values not return table
  local filename, linedefined = odebug.info()

  result[filename] = result[filename] or {}
  if result[filename][linedefined] then
    result[filename][linedefined] = result[filename][linedefined] + 1
  else
    result[filename][linedefined] = 1
  end
end

M.start = function()
  debug.sethook(hook, "l")
end

-------------------------------------------------------------------------------
-- STOP
-- sanitize file name, remove @ char, and replace for absolute path
-------------------------------------------------------------------------------

M.stop = function()
  local tmp = {}
  for fileName, content in pairs(result) do
    if fileName:startsWith('@') then
      fileName = fileName:mid(1, -1)
    end
    fileName = os.abspath(fileName)
    tmp[fileName] = content
  end
  result = tmp
  debug.sethook(nil, "l")
end

M.dump = function()
  return result
end

M.reset = function()
  result = {}
end

M.line = function(line, flag)

  if M.flag then
    flag = -1
  end

  if line:gsub(' ', ''):startsWith("--") then
    flag = -1
  end

  if line:endsWith("end") then
    flag = -1
  end

  if line:endsWith("else") then
    flag = -1
  end

  if line:endsWith("(") then
    M.flag = true
  end

  if (line:contains("{") and not line:contains("}")) then
    M.flag = true
  end

  --if line:endsWith(")") then
  --  flag = 1
  --end

  if line:endsWith("}") then
    M.flag = false
  end

  if line == "}" then
    flag = -1
  end

  if line == '' then
    flag = -1
  end

  return flag
end

M.analyze = function(file_name)
  local data  = {}
  local lines = {}
  local count = 1
  local uncov = 0

  if file_name:startsWith('@') then
    file_name = file_name:mid(1, -1)
  end

  file_name = os.abspath(file_name)

  if result[file_name] == nil then
    result[file_name] = {}
  end

  for line in io.lines(file_name:replace('@', '')) do
    flag = result[file_name][count]
    flag = M.line(line, flag)
    if flag == nil then
      uncov = uncov + 1
    end
    lines[count] = { src = line, flag = flag }
    count = count + 1
  end

  data.lines     = lines
  data.file_name = file_name
  data.coverage  = 100 - ((100 * uncov) / count)
  M.flag = nil

  return data
end

return M
