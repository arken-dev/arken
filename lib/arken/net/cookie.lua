-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local M = {}

M.parse = function(buffer)
  local result   = {}
  local pos      = 0
  local len      = buffer:len()
  local index    = buffer:find(";")
  if index == nil and len > 0 then
    local equal    = buffer:find("=")
    local key      = buffer:sub(0, equal-1)
    local value    = (buffer:sub(equal+1, buffer:len())):trim()
    result[key] = value
    return result
  end
  while(index ~= nil) do
    local fragment = buffer:sub(0, index)
    local equal    = fragment:find("=")
    local key      = fragment:sub(0, equal-1)
    local value    = (fragment:sub(equal+1, index-1)):trim()
    result[key] = value
    --
    buffer = buffer:sub(index+2, -1)
    index  = buffer:find(";")
    if index == nil then
      fragment = buffer:sub(0, index)
      equal    = fragment:find("=")
      key      = fragment:sub(0, equal-1)
      value    = (fragment:sub(equal+1, -1)):trim()
      result[key] = value
    end
  end

  return result
end

return M
