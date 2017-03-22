-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

-- Object
-- primitives ajustes for main objects
Object = Object or {}

------------------------------------------------------------------------------
-- METAMETHOD __index
------------------------------------------------------------------------------

Object.__index = Object

------------------------------------------------------------------------------
-- METAMETHOD __newindex
------------------------------------------------------------------------------

Object.__newindex = function(t, k, v)
  rawset(t, k, v)
  if k == 'inherit' and v ~= nil then
    v(t)
  end
end

------------------------------------------------------------------------------
-- CONSTRUTOR
------------------------------------------------------------------------------

function Object:initialize()
  -- print('initialize Object')
end

------------------------------------------------------------------------------
-- GENERIC CONTRACT
------------------------------------------------------------------------------

function Object:prepare(params)
end

function Object:validate(params)
end

function Object:before(params)
end

function Object:after(params)
end

function Object:execute(method, params)
  self:prepare(params)
  self:validate(params)
  self:before(params)
  local result = self[method](self, params)
  self:after(params)
  return result
end

------------------------------------------------------------------------------
-- SPECIFIC CONTRACT
------------------------------------------------------------------------------
--[[
function Object:contract(name)
  local contract = require('contract')
  contract.create(self, name)
end
]]
------------------------------------------------------------------------------
-- TRY
------------------------------------------------------------------------------

function Object:try(column, params)
  local result = nil;
  if type(self[column]) == 'function' then
    result = self[column](self, params)
  else
    result = self[column]
  end
  if result == nil then
    result = Object.new()
  end
  return result
end

------------------------------------------------------------------------------
-- NEW
------------------------------------------------------------------------------

function Object.new(record)
  local obj = record or {}
  setmetatable(obj, Object)
  obj:initialize()
  return obj
end

------------------------------------------------------------------------------
-- IS BLANK
------------------------------------------------------------------------------
--[[
function Object:isBlank(column)
  if self[column] == nil or self[column] == '' then
    return true
  else
    return false
  end
end
]]
------------------------------------------------------------------------------
-- METHODS
------------------------------------------------------------------------------

function Object:methods()
  local methods = {}
  for k, v in pairs(self.class) do
    if type(v) == 'function' then
      table.insert(methods, k)
    end
  end
  table.sort(methods)
  return methods
end

function Object:__tostring()
  return ""
end

function Object:pcall(method, ...)
  return pcall(self[method], self, ...)
end

return Object
