local dispatcher = require 'arken.dispatcher'

return function(env)
  return dispatcher.dispatch(env)
end
