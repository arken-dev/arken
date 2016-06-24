-------------------------------------------------------------------------------
-- SHOULD MODULE
-------------------------------------------------------------------------------

local should = {}

function should.be_valid(true_value)
  if(true_value) then
    return true
  else
    local message = 'expected true value'
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.not_valid(false_value)
  if(false_value == false) then
    return true
  else
    local message = 'expected true value'
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_type(value, str)
  if(type(value) == str) then
    return true
  else
    local message =  type(value) .. ' is not type ' .. tostring(str)
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_equal(val1, val2)
  if(val1 == val2) then
    return true
  else
    local message =  tostring(val1) .. ' is not equal to ' .. tostring(val2)
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.not_equal(val1, val2)
  if(val1 ~= val2) then
    return true
  else
    local message =  tostring(val1) .. ' is not equal to ' .. tostring(val2)
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_nil(value)
  if value == nil then
    return true
  else
    local message = 'expected nil value'
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_contains(value, contains)
  if type(value) == 'string' then
    if value:contains(contains) then
      return true
    else
      local message = string.format('%s not contains %s', value, contains)
      error({code = 2000, msg = message, kind = 'test'})
    end
  else
    local message = 'value is not string'
    error({code = 2000, msg = message, kind = 'test'})
  end
end


return should
