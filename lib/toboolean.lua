function toboolean(value)
  if value == 'true' or value == 't' or value == true or value == 1 then
    return true
  end

  if value == 'false' or value == 'f' or value == false then
    return false
  end

  return nil
end
