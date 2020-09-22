-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local contract = {}

--------------------------------------------------------------------------------
-- CLEAR
-- t is null when static methods without params
--------------------------------------------------------------------------------

contract.clear = function(t)
  if t and type(t.errors) == 'table' then
    for key, _ in pairs(t.errors) do
      t.errors[key] = nil
    end
  end
end

--------------------------------------------------------------------------------
-- BANG !
--------------------------------------------------------------------------------
contract.bang = function(errors)
  if type(errors) == 'table' then
    local flag = false
    for k, v in pairs(errors) do
      flag = true
      break
    end
    if flag then
      errors.traceback = debug.traceback()
      error(errors)
    end
  end
end

--------------------------------------------------------------------------------
-- CREATE
--------------------------------------------------------------------------------

-- deve emitir erro se nao tiver dois parametros
contract.create = function(table, name)

  local prepare  = name .. 'Prepare'
  local validate = name .. 'Validate'
  local before   = name .. 'Before'
  local body     = name .. "Body"
  local after    = name .. "After"

  -- check if not exists contract_prepare
  if table[prepare] == nil then
    table[prepare] = function(self, ...) end
  end

  -- check if not exists contract_validate
  if table[validate] == nil then
    table[validate] = function(self, ...) end
  end

  -- check if not exists contract_before
  if table[before] == nil then
    table[before] = function(self, ...) end
  end

  -- check if not exists contract_before
  if table[after] == nil then
    table[after] = function(self, ...) end
  end

  -- contract is now "contract_body"
  local func = table[name]
  table[body] = func

  -- contract current
  table[name] = function(self, ...)
    contract.clear(self)
    table[prepare](self, ...)
    local errors = table[validate](self, ...) or self.errors
    contract.bang(errors)
    table[before](self, ...)
    local result = table[body](self, ...)
    table[after](self, ...)
    return result
  end

end

return contract
