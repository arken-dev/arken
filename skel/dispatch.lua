local dispatcher = require 'charon.dispatcher'

function dispatch(env)
  return dispatcher.dispatch(env)
end
