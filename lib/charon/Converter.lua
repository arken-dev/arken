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
    return next(value) == nil
  end

  return false
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
    return false
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
      value = value:replaceChar('.', ''):replaceChar(',', '.')
    end

    return tonumber(value) or default
  end
end

-------------------------------------------------------------------------------
-- INSPECT
-------------------------------------------------------------------------------

function Converter:inspect(field)
  local value
  if field then
    value = self[field]
  else
    value = self
  end
  if type(value) == 'userdata' then
    return 'userdata'
  else
    return json.encode(value)
  end
end

return Converter
