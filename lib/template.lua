local M = {}

M.cache = {}

function M.compile(file_name)
  local data = os.read(file_name)

  local len  = data:len()
  local flag = false
  local i    = 0
  local j    = 0

  local buffer =
[[
return function(self)
  local __buffer = ''
   __buffer = __buffer .. [[
]]

  while i < len do

    if data:sub(i, i+2) == '<%=' then
      buffer = buffer .. "]] .. "
      flag = true
      i = i + 3
    elseif data:sub(i, i+1) == '<%'  then
      buffer = buffer .. "]] "
      i = i + 2
    elseif data:sub(i, i+2) == '-%>'  then
      if flag then
        buffer = buffer .. " .. [["
        flag   = false
      else
        buffer = buffer .. "\n __buffer = __buffer .. [["
      end
      i = i + 2
    elseif data:sub(i, i+1) == '%>'  then
      if flag then
        buffer = buffer .. " .. [[\n"
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

  buffer = buffer .. "]] \n"
  buffer = buffer .. "  return __buffer \n"
  buffer = buffer .. "end"

  return buffer
end

function M.build(file_name)
  return assert(loadstring(M.compile(file_name)))
end

function M.execute(file_name, data)
  if not M.cache[file_name] then
    M.cache[file_name] = M.build(file_name)()
  end
  return M.cache[file_name](data)
end

function M.reload()
  M.cache = {}
end

return M
