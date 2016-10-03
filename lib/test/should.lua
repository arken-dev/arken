-------------------------------------------------------------------------------
-- SHOULD MODULE
-------------------------------------------------------------------------------

local should = {}

function should.be_valid(true_value, message)
  if(true_value) then
    return true
  else
    message = message or 'expected true value'
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.not_valid(false_value, message)
  if(false_value == false) then
    return true
  else
    message = message or 'expected true value'
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_type(value, str, message)
  if(type(value) == str) then
    return true
  else
    message =  message or type(value) .. ' is not type ' .. tostring(str)
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_equal(val1, val2, message)
  if(val1 == val2) then
    return true
  else
    message = message or tostring(val1) .. ' is not equal to ' .. tostring(val2)
    error({code = 2000, msg = message, kind = 'test', traceback = debug.traceback()})
  end
end

function should.not_equal(val1, val2, message)
  if(val1 ~= val2) then
    return true
  else
    message = message or tostring(val1) .. ' is not equal to ' .. tostring(val2)
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_nil(value, message)
  if value == nil then
    return true
  else
    message = message or 'expected nil value'
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.be_contains(value, contains, message)
  if type(value) == 'string' then
    if value:contains(contains) then
      return true
    else
      message = message or string.format('%s not contains %s', value, contains)
      error({code = 2000, msg = message, kind = 'test'})
    end
  else
    local message = 'value is not string'
    error({code = 2000, msg = message, kind = 'test'})
  end
end

function should.fail(value)
  local message = inspect(value)
  error({code = 2000, msg = message, kind = 'test'})
end

return should
