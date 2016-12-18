local cache  = {}
function using(module)
  local data = cache[module]
  if not data then
    data  = {}
    data.name  = module:mid(module:lastIndexOf('.') + 1, -1)
    data.value = require(module)
    cache[module] = data
  end
  local env   = getfenv(2)
  env[data.name] = data.value
  return data.value
end
