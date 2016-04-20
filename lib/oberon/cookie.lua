require "OByteArray"

local M = {}

M.parse = function(buffer)
  local result   = {}
  local pos      = 0
  local len      = buffer:len()
  local index    = buffer:find(";")
  while(index ~= nil) do
    local fragment = buffer:sub(0, index)
    local equal    = fragment:find("=")
    local key      = fragment:sub(0, equal-1)
    local value    = OByteArray.new(fragment:sub(equal+1, index-1)):trimmed()
    result[key] = value
    --
    buffer = buffer:sub(index+2, -1)
    index  = buffer:find(";")
    if index == nil then
      fragment = buffer:sub(0, index)
      equal    = fragment:find("=")
      key      = fragment:sub(0, equal-1)
      value    = OByteArray.new(fragment:sub(equal+1, -1)):trimmed()
      result[key] = value
    end
  end

  return result
end

return M