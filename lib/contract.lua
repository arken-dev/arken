local Contract = {}
-- deve emitir erro se nao tiver dois parametros
Contract['create'] = function(table, contract)

  local prepare  = contract .. 'Prepare'
  local validate = contract .. 'Validate'
  local before   = contract .. 'Before'
  local body     = contract .. "Body"
  local after    = contract .. "After"

  -- check if not exists contract_prepare
  if table[prepare] == nil then
    table[prepare] = function(t, params) end
  end

  -- check if not exists contract_validate
  if table[validate] == nil then
    table[validate] = function(t, params) end
  end

  -- check if not exists contract_before
  if table[before] == nil then
    table[before] = function(t, params) end
  end

  -- check if not exists contract_before
  if table[after] == nil then
    table[after] = function(t, params) end
  end

  -- contract is now "contract_body"
  func = table[contract]
  table[body] = func

  -- contract current
  table[contract] = function(t, params)
    table[prepare](t, params)
    table[validate](t, params)
    table[before](t, params)
    local result = table[body](t, params)
    table[after](t, params)
    return result
  end

end

return Contract
