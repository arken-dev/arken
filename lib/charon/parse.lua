-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

parse = {}

-------------------------------------------------------------------------------
-- IS EMPTY
-------------------------------------------------------------------------------

parse.isEmpty = function( value )
  if value == nil then
    return true
  end

  if value == '' then
    return true
  end

  return false
end

-------------------------------------------------------------------------------
-- IS NUMBER
-------------------------------------------------------------------------------


parse.isNumber = function(value)
  if value == nil then
    return false
  end

  if type(value) == 'number' then
    return true
  end

  if type(value) == 'string' then
    if value:contains(',') then
      value = value:replace('.', ''):replace(',', '.')
    end

    return not (tonumber(value) == nil)
  end

  return false
end

-------------------------------------------------------------------------------
-- IS BOOLEAN
-------------------------------------------------------------------------------

parse.isBoolean = function( value )
  if value == nil then
    return false
  end

  if type(value) == 'boolean' then
    return true
  end

  if type(value) == 'string' then
    return ( value == 'true' or value == 'false' or value == 't' or value == 'f' or value == '1' or value == '0' )
  end

  return false
end

-------------------------------------------------------------------------------
-- IS DATE TIME
-------------------------------------------------------------------------------

parse.isDateTime = function()
  if tostring(value):sub(6, 6) == '/' or tostring(value):sub(5, 5) == '/' then
    return value:sub(14, 14) == ':'
  end

  return false
end

-------------------------------------------------------------------------------
-- IS DATE
-------------------------------------------------------------------------------

parse.isDate = function()
  return tostring(value):sub(6, 6) == '/' or tostring(value):sub(5, 5) == '/'
end

-------------------------------------------------------------------------------
-- IS TIME
-------------------------------------------------------------------------------

parse.isTime = function()
  return tostring(value):sub(3, 3) == ':'
end

-------------------------------------------------------------------------------
-- TO BOOLEAN
-------------------------------------------------------------------------------

parse.toBoolean = function(value)
  value = tostring(value)
  if value == 'true' or value == 't' or value == '1' then
    return true
  end

  return false
end

-------------------------------------------------------------------------------
-- TO NUMBER
-------------------------------------------------------------------------------

parse.toNumber = function(value)
  if type(value) == 'number' then
    return value
  else
    value = tostring(value)
    if value:contains(',') then
      value = value:replace('.', ''):replace(',', '.')
    end

    return tonumber(value) or 0
  end
end

-------------------------------------------------------------------------------
-- INSPECT
-------------------------------------------------------------------------------

parse.inspect = function(value)
  if type(value) == 'userdata' then
    return 'userdata'
  else
    return json.encode(value)
  end
end

return parse
