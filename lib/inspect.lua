function inspect(value)
  if type(value) == 'userdata' then
    return 'userdata'
  else
    return json.encode(value)
  end
end
