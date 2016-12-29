-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Class = require('charon.oop.Class')
local json  = require('charon.json')
local Converter = Class.new("Converter")

-------------------------------------------------------------------------------
-- IS EMPTY
-------------------------------------------------------------------------------

function Converter:isEmpty( field )
  local value = self[field]
  if value == nil then
    return true
  end

  if value == '' then
    return true
  end

  if type(value) == 'table' then
    local flag = true
    for _, _ in pairs(value) do
      flag = false
      break
    end
    return flag
  end

  return false
end

-------------------------------------------------------------------------------
-- IS NUMBER
-------------------------------------------------------------------------------


function Converter:isNumber(field)
  local value = self[field]

  if value == nil then
    return false
  end

  if type(value) == 'number' then
    return true
  end

  if type(value) == 'string' then
    if value:contains(',') then
      value = value:replaceChars('.', ''):replaceChars(',', '.')
    end

    return not (tonumber(value) == nil)
  end

  return false
end

-------------------------------------------------------------------------------
-- IS BOOLEAN
-------------------------------------------------------------------------------

function Converter:isBoolean( field )
  local value = self[field]

  if value == nil then
    return false
  end

  if type(value) == 'boolean' then
    return true
  end

  if type(value) == 'string' then
    if value == 'true' or value == 't' or value == '1' then
      return true
    end
    if value == 'false' or value == 'f' or value == '0' then
      return true
    end
  end

  return false
end

-------------------------------------------------------------------------------
-- IS BOOL
-------------------------------------------------------------------------------

Converter.isBool = Converter.isBoolean

-------------------------------------------------------------------------------
-- IS DATE TIME
-------------------------------------------------------------------------------

function Converter:isDateTime(field)
  local value = self[field]

  if tostring(value):sub(6, 6) == '/' or tostring(value):sub(5, 5) == '/' then
    return value:sub(14, 14) == ':'
  end

  return false
end

-------------------------------------------------------------------------------
-- IS DATE
-------------------------------------------------------------------------------

function Converter:isDate(field)
  local value = self[field]
  return tostring(value):sub(6, 6) == '/' or tostring(value):sub(5, 5) == '/'
end

-------------------------------------------------------------------------------
-- IS TIME
-------------------------------------------------------------------------------

function Converter:isTime(field)
  local value = self[field]
  return tostring(value):sub(3, 3) == ':'
end

-------------------------------------------------------------------------------
-- TO BOOLEAN
-------------------------------------------------------------------------------

function Converter:toBoolean(field, default)
  local value = tostring(self[field])

  if value == 'true' or value == 't' or value == '1' then
    return true
  end

  if value == 'false' or value == 'f' or value == '0' then
    return true
  end
  return default
end

-------------------------------------------------------------------------------
-- TO NUMBER
-------------------------------------------------------------------------------

function Converter:toNumber(field, default)
  local value = self[field]
  if type(value) == 'number' then
    return value
  else
    value = tostring(value)
    if value:contains(',') then
      value = value:replaceChars('.', ''):replaceChars(',', '.')
    end

    return tonumber(value) or default
  end
end

-------------------------------------------------------------------------------
-- INSPECT
-------------------------------------------------------------------------------

function Converter.inspect(field)
  local value = field or self
  if type(value) == 'userdata' then
    return 'userdata'
  else
    return json.encode(value)
  end
end

return Converter
