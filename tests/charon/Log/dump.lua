local test = {}
local Log  = require('charon.Log')

test.should_write_multiples_logs_instances_in_unique_file = function()
  local tmp  = os.tmpname()
  local log1 = Log.new(tmp)
  local log2 = Log.new(tmp)

  log1:append('teste1')
  log2:append('teste2')
  log1:append('teste3')

  log1:dump()

  local file = io.open(tmp)
  local row  = nil

  row = file:read()
  assert( row == 'teste1', row )

  row = file:read()
  assert( row == 'teste2', row )

  row = file:read()
  assert( row == 'teste3', row )

  row = file:read()
  assert( row == nil, row )
end

return test
