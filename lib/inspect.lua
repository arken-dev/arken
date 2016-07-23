function inspect(value)
  if type(value) == 'userdata' then
    return 'userdata'
  else
    local JSON = require('JSON')
    return JSON:encode(value)
  end
end
