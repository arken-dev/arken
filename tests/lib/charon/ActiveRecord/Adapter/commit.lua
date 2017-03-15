local ActiveRecord = require('charon.ActiveRecord')
local test = {}

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_execute_commit_after_begin = function()
  local adapter = ActiveRecord.factoryAdapter()
  adapter:begin()
  local value = adapter:commit()
  assert(type(value) == 'number', value)
end

test.should_clear_errors = function()
  local Adapter = require('charon.ActiveRecord.Adapter')
  local adapter = ActiveRecord.factoryAdapter()
  Adapter.errors.value = 'true'
  adapter:begin()
  assert(Adapter.errors.value == 'true')
  adapter:commit()
  assert(Adapter.errors.value == nil, tostring(adapter.errors.value))
end

test.should_clear_cache = function()
  local Adapter = require('charon.ActiveRecord.Adapter')
  local adapter = ActiveRecord.factoryAdapter()
  Adapter.cache.value = 'true'
  adapter:begin()
  assert(Adapter.cache.value == 'true')
  adapter:commit()
  assert(Adapter.cache.value == nil, adapter.cache.value)
end

test.should_clear_neat = function()
  local Adapter = require('charon.ActiveRecord.Adapter')
  local adapter = ActiveRecord.factoryAdapter()
  Adapter.neat.value = 'true'
  adapter:begin()
  assert(Adapter.neat.value == 'true')
  adapter:commit()
  assert(Adapter.neat.value == nil, adapter.cache.value)
end

test.should_error_if_not_begin = function()
  local status, message = pcall(ActiveRecord.commit)
  assert(status == false)
  assert(message:contains('cannot commit - no transaction is active'), message)
end

return test
