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
      fileName = fileName:mid(2, -1)
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

coverage.line = function(row, flag)
  local line = row:trimmed()
  local result = nil
  coverage.comment = nil

  if line == '' then
    coverage.comment = "line is blank"
    return -1
  end

  if coverage.default == -1 and line:contains("]]") and line:contains("[[") then
    if line:indexOf('[[') > line:indexOf(']]') then
      coverage.comment = [[starts with --[[ or endsWith [[ or endsWith {]]
      coverage.default = -1
      return -1
    end
  end

  if coverage.default == nil and (line:contains("--[[") or line:contains("[[")) then
    coverage.comment = [[starts with --[[ or endsWith [[]]
    coverage.default = -1
    if result == nil then
      result = 1
    else
      result = -1
    end
  end

  if coverage.default == -1 and (line:contains("]]") or line == "]]") then
    coverage.comment = "end with ]]"
    coverage.default = nil
    if coverage.level == 0 or line == ']]' then
      result = 1
    else
      return flag
    end
  end

  if result ~= nil then
    return result
  end

  if line:startsWith("--") then
    coverage.comment = "starts with --"
    return -1
  end

  if coverage.default == -1 then
    coverage.comment = "default is -1"
    return -1
  end

  if coverage.default == nil then
    coverage.comment = "default is nil"

    if coverage.braces == true and line:endsWith("}") then
      coverage.braces = false
      return 1
    end

    if line:endsWith("{") or coverage.braces == true or (line:contains('{') and not line:contains('}')) then
      coverage.braces = true
      return 1
    end

    if line:contains('function') then
      coverage.level = coverage.level + 1
      return 1
    end

    if line:endsWith(' do') or line == 'do' then
      coverage.level = coverage.level + 1
      return 1
    end

    if line:endsWith(' then') or line == 'then' then
      coverage.level = coverage.level + 1
      return 1
    end

    if line:endsWith(' else') or line == 'else' then
      return 1
    end

    if line:endsWith(' end') or line == 'end' then
      coverage.level = coverage.level + -1
      return 1
    end
  end

  if coverage.default == nil and row:startsWith('return') then
    coverage.comment = "default is nil and start with return"
    return 1
  end

  if coverage.level == 0 then
    coverage.comment = "level is 0"
    return 1
  end

  coverage.comment = "returning flag"
  return flag
end

function coverage.analyze(file_name)
  local data  = {}
  local lines = {}
  local count = 1
  local uncov = 0
  local level = 0
  local default = 1

  if file_name:startsWith('@') then
    file_name = file_name:mid(2, -1)
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
  --local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  local i = 0
  coverage.default = nil
  coverage.braces  = false
  coverage.level = 0
  for line in io.lines(file_name) do
    i = i + 1
    flag = result[file_name][count]
    -- debug
    -- print("number :", i, "flag ", flag, "line : ", line)
    flag = coverage.line(line, flag)
    if flag == nil then
      uncov = uncov + 1
    end
    lines[count] = {
      src = line, flag = flag, level = coverage.level,
      default = coverage.default, comment = coverage.comment
    }
    count = count + 1
  end

  data.lines     = lines
  data.file_name = file_name
  data.coverage  = 100 - ((100 * uncov) / count)

  return data
end

return coverage
