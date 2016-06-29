function isblank(value)
  if type(value) == 'nil' then
    return true
  end

  if type(value) == 'string' then
     return #value == 0
  end

  if type(value) == 'table' then
    return next(value) == nil
  end

  if type(value) == 'boolean' then
    return value == false
  end

  return false
end
