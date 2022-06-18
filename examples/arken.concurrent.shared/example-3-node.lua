local mvm = require('arken.mvm')

return function(task, params)
  task:shared():put('hello', 'world!******')
  print(mvm.current():version())
  mvm.current():shared():append('hello', ' yes!!!')
  local instance = mvm.current()
  local shared   = instance:shared()
  shared:append("hello", " ")
  shared:append('hello', "it's works!!!")
end
