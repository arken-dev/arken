function toboolean(value)
  if value == nil then
    return nil
  end

  if value == 'true' or value == 't' or value == true then
    return true
  end

  return false
end
