-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local template  = {}
template.cache  = {}
template.source = {}
template.mtime  = {}
template.time   = 0

function template.paramsToLocal(params)
  local result = ""
  for k, _ in pairs(params) do
    result = result .. string.format("local %s = params.%s\n", k, k)
  end
  return result
end

function template.compile(file_name, params)
  if not os.exists(file_name) then
    error(string.format("%s not exists", file_name))
  end
  local data = os.read(file_name)

  local len  = data:len()
  local flag = false
  local i    = 0
  local j    = 0

  local buffer = ""
  buffer = buffer .. "return function(params)\n"
  buffer = buffer .. template.paramsToLocal(params)
  buffer = buffer .. "  local __buffer = ''\n"
  buffer = buffer .. "   __buffer = __buffer .. [[\n"

  while i < len do

    if data:sub(i, i+2) == '<%=' then
      buffer = buffer .. "]] .. tostring("
      flag = true
      i = i + 3
    elseif data:sub(i, i+1) == '<%'  then
      buffer = buffer .. "]]\n"
      i = i + 2
    elseif data:sub(i, i+2) == '-%>'  then
      if flag then
        buffer = buffer .. ") .. [["
        flag   = false
      else
        buffer = buffer .. "\n __buffer = __buffer .. [["
      end
      i = i + 2
    elseif data:sub(i, i+1) == '%>'  then
      if flag then
        buffer = buffer .. ") .. [[\n"
        flag   = false
      else
        buffer = buffer .. "\n __buffer = __buffer .. [["
      end
      i = i + 1

    else
      if data:sub(i, i) == '\n' then
        j = i + 1
        while data:sub(j, j) == ' ' do
          j = j + 1
        end
        if data:sub(j, j+2) == '<% ' then
          buffer = buffer .. data:sub(i, i)
          i = j - 1
        else
          buffer = buffer .. data:sub(i, i)
        end
      else
        buffer = buffer .. data:sub(i, i)
      end
    end
    i = i + 1

  end

  buffer = buffer .. "]]\n"
  buffer = buffer .. "  return __buffer\n"
  buffer = buffer .. "end\n"
  return template.filter(file_name, buffer)
end

function template.filter(file_name, buffer)
  return buffer
end

function template.build(file_name, params)
  template.source[file_name] = template.compile(file_name, params)
  return assert(loadstring(template.source[file_name]))
end

function template.execute(file_name, params, flag)
  local buffer = nil
  local time   = os.microtime()
  local mtime  = template.mtime[file_name] or 0
  if flag and os.ctime(file_name) > mtime then
    template.cache[file_name] = nil
  end
  if not template.cache[file_name] then
    template.cache[file_name] = template.build(file_name, params)()
    template.mtime[file_name] = os.microtime()
  end
  buffer = template.cache[file_name](params)
  template.time = template.time + (os.microtime() - time)
  return buffer
end

function template.debug(file_name)
  if not os.exists(file_name) then
    error(string.format("file %s not exists", file_name))
  end
  local source = template.source[file_name]
  local buffer = ""
  local list   = source:split('\n')
  for i = 1, list:size() do
    if i < 10 then
      buffer = buffer .. ' '
    end
    buffer = buffer .. i .. ") " .. list:at(i) .. '\n'
  end
  return buffer
end

return template
