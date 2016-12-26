-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local odebug   = require 'charon.odebug'
local coverage = {}
local result   = {}

coverage.hook = function(val1, val2, fake)
  -- disabling because it is slow
  --[[
  local info = debug.getinfo(2, "Sl")
  local filename     = info.source
  local linedefined  = info.currentline
  ]]

  -- odebug is a charon module return values whitout table
  local ldebug = fake or odebug
  local filename, linedefined = ldebug.info()

  result[filename] = result[filename] or {}
  if result[filename][linedefined] then
    result[filename][linedefined] = result[filename][linedefined] + 1
  else
    result[filename][linedefined] = 1
  end
end

coverage.start = function(fake)
  local debug = fake or require('debug')
  debug.sethook(coverage.hook, "l")
end

-------------------------------------------------------------------------------
-- STOP
-- sanitize file name, remove @ char, and replace for absolute path
-------------------------------------------------------------------------------

coverage.stop = function(fake)
  local debug = fake or require('debug')
  debug.sethook(nil, "l")
  local tmp = {}
  for fileName, content in pairs(result) do
    if fileName:startsWith('@') then
      fileName = fileName:mid(1, -1)
    end
    fileName = os.abspath(fileName)
    tmp[fileName] = content
  end
  result = tmp
end

coverage.dump = function()
  return result
end

coverage.reset = function()
  result = {}
end

coverage.line = function(line, flag, keywords)
  local trimmed = line:trimmed()

  if trimmed == 'else' then
     return -1
   end

  if trimmed == 'end' then
     return -1
  end

  if trimmed == '' then
     return -1
  end

  if trimmed:startsWith("--") then
    if trimmed:startsWith("--[[") then
      flag = -1
    else
      return -1
    end
  end

  if keywords.flag == nil and keywords.str1 == false and keywords.str2 == false then
    local index  = line:indexOf("function")
    if index > -1 then
      local simplified = line:simplified()
      if simplified:mid(index-1, 1) == '=' or simplified:mid(index-2, 1) == '=' then
        keywords.flag = false
        flag = 1
      end
      return flag
    end
  end

  for i=1, #line do
    local chr = line:sub(i,i)
    if keywords.str2 == false and chr == "'" then
      if keywords.str1 then
        keywords.str1 = false
      else
        keywords.str1 = true
      end
    end

    if keywords.str1 == false and chr == '"' then
      if keywords.str2 then
        keywords.str2 = false
      else
        keywords.str2 = true
      end
    end

    if keywords.str1 == false and keywords.str2 == false then
      if chr == '(' then
        keywords.flag1 = keywords.flag1 + 1
      end
      if chr == ')' then
        keywords.flag1 = keywords.flag1 - 1
      end
      if chr == '{' then
        keywords.flag2 = keywords.flag2 + 1
      end
      if chr == '}' then
        keywords.flag2 = keywords.flag2 - 1
      end
      if chr == '[' then
        keywords.flag3 = keywords.flag3 + 1
      end
      if chr == ']' then
        keywords.flag3 = keywords.flag3 - 1
      end
    end
  end

  if keywords.str1 or keywords.str2 then
    return -1
  end

  if keywords.flag1 > 0 or keywords.flag2 > 0 or keywords.flag3 > 0 then
    if keywords.flag then
      return flag or -1
    else
      keywords.flag = true
      return flag or 1
    end
  else
    if keywords.flag then
      keywords.flag = false
      return flag or -1
    else
      return flag
    end
  end
  return flag
end

function coverage.analyze(file_name)
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

  -- keywords
  -- flag1 = ( )
  -- flag2 = { }
  -- flag3 = [ ]
  -- str1  = '
  -- str2  = "
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  local i = 0
  for line in io.lines(file_name) do
    i = i + 1
    flag = result[file_name][count]
    flag = coverage.line(line, flag, keywords)
    if flag == nil then
      uncov = uncov + 1
    end
    lines[count] = { src = line, flag = flag }
    count = count + 1
  end

  data.lines     = lines
  data.file_name = file_name
  data.coverage  = 100 - ((100 * uncov) / count)

  return data
end

return coverage
