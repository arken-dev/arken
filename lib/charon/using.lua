function using(module, name)
  local index = module:lastIndexOf('.') + 1
  local name  = name or module:mid(index, -1)
  local value = require(module)
  local env   = getfenv(2)
  env[name] = value
end
