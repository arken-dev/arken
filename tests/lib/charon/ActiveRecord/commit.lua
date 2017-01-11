local ActiveRecord = require('charon.ActiveRecord')
local test = {}

test.before = function()
  ActiveRecord.config = "config/active_record_sqlite.json"
end

test.after = function()
  ActiveRecord.config = nil
end

test.should_execute_commit_after_begin = function()
  ActiveRecord.begin()
  ActiveRecord.commit()
end

test.should_error_if_not_begin = function()
  local status, message = pcall(ActiveRecord.commit)
  assert(status == false)
  assert(message:contains('cannot commit - no transaction is active'), message)
end

return test
