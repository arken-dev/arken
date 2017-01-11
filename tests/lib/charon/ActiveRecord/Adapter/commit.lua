local ActiveRecord = require('charon.ActiveRecord')
local test    = {}

test.before = function()
  ActiveRecord.config = "config/active_record_sqlite.json"
end

test.after = function()
  ActiveRecord.config = nil
end

test.should_execute_commit_after_begin = function()
  local adapter = ActiveRecord.loadAdapter()
  adapter:begin()
  local value = adapter:commit()
  assert(value == 0, value)
end

test.should_clear_errors = function()
  local adapter = ActiveRecord.loadAdapter()
  adapter.errors.value = 'true'
  adapter:begin()
  assert(adapter.errors.value == 'true')
  adapter:commit()
  assert(adapter.errors.value == nil)
end

test.should_clear_cache = function()
  local adapter = ActiveRecord.loadAdapter()
  adapter.cache.value = 'true'
  adapter:begin()
  assert(adapter.cache.value == 'true')
  adapter:commit()
  assert(adapter.cache.value == nil, adapter.cache.value)
end

test.should_clear_neat = function()
  local adapter = ActiveRecord.loadAdapter()
  adapter.neat.value = 'true'
  adapter:begin()
  assert(adapter.neat.value == 'true')
  adapter:commit()
  assert(adapter.neat.value == nil, adapter.cache.value)
end

test.should_error_if_not_begin = function()
  local status, message = pcall(ActiveRecord.commit)
  assert(status == false)
  assert(message:contains('cannot commit - no transaction is active'), message)
end

return test
