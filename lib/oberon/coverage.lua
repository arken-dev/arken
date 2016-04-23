require 'oberon.odebug'
local M = {}

local result = {}

local hook = function()
  -- disabling because it is slow
  --[[
  local info = debug.getinfo(2, "Sl")
  local filename     = info.source
  local linedefined  = info.currentline
  ]]

  -- odebug is a oberon module return values
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

M.stop = function()
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

  if line:endsWith("{") then
    M.flag = true
  end

  if line:endsWith(")") then
    M.flag = false
    --flag = -1
  end

  if line:endsWith("}") then
    M.flag = false
    --flag = -1
  end

  if line == '' then
    flag = -1
  end

  return flag
end

M.analyze = function(file_name)
  file_name = file_name:replace('@', '')
  local analyze = {}
  local count   = 1
  local flag
  local uncoverage = 0
  for line in io.lines(file_name) do
    flag = result['@' .. file_name][count]
    flag = M.line(line, flag)
    if flag == nil then
      uncoverage = uncoverage + 1
    end
    analyze[count] = { line = line, flag = flag }
    -- use for debug
    -- print(tostring(flag) .. ')' .. line)
    count = count + 1
  end
  analyze.coverage = 100 - ((100 * uncoverage) / count)
  M.flag = nil
  return analyze
end

return M
