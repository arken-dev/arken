-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local odebug   = require 'arken.odebug'
local coverage = {}
local result   = {}

coverage.parens = 0
coverage.opened = nil

coverage.hook = function(val1, val2, fake)
  -- disabling because it is slow
  --[[
  local info = debug.getinfo(2, "Sl")
  local filename     = info.source
  local linedefined  = info.currentline
  ]]

  -- odebug is a arken module return values whitout table
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

-------------------------------------------------------------------------------
-- BALANCE
-- count open parenthesis, ignoring the ones inside strings and comments
-------------------------------------------------------------------------------

coverage.balance = function(row)
  local total = 0
  local quote = nil
  local index = 1

  while index <= row:len() do
    local char = row:sub(index, index)
    if quote then
      if char == '\\' then
        index = index + 1
      elseif char == quote then
        quote = nil
      end
    elseif char == '"' or char == "'" then
      quote = char
    elseif char == '-' and row:sub(index + 1, index + 1) == '-' then
      break
    elseif char == '[' and row:sub(index + 1, index + 1) == '[' then
      local close = row:indexOf(']]', index + 2)
      if close < 0 then
        break
      end
      index = close + 1
    elseif char == '(' then
      total = total + 1
    elseif char == ')' then
      total = total - 1
    end
    index = index + 1
  end

  return total
end

-------------------------------------------------------------------------------
-- LINE
-- lines that only continue an open parenthesis are not a statement by
-- themselves, the vm never reports them, so they follow the line that opened it
-------------------------------------------------------------------------------

coverage.line = function(row, flag)
  local continues = coverage.parens > 0
  local result    = coverage.evaluate(row, flag)

  if coverage.default == nil then
    coverage.parens = math.max(0, coverage.parens + coverage.balance(row))
  end

  if continues then
    if result == -1 then
      return result
    end
    coverage.comment = "continues open parenthesis"
    if flag == nil then
      return coverage.opened
    end
    return flag
  end

  if coverage.parens > 0 and row:contains('function') then
    coverage.parens = 0
  end

  if coverage.parens > 0 then
    coverage.opened = result
  end

  return result
end

coverage.evaluate = function(row, flag)
  local line = row:trim()
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

    if line == '}' or line == ')' then
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

    if line == 'break' then
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
  coverage.parens = 0
  coverage.opened = nil
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
