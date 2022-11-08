local dispatcher = require 'arken.dispatcher'

return function(env)
  print(env)
  return dispatcher.dispatch(env)
end
