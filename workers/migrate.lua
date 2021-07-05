ARKEN_ENV = os.getenv("ARKEN_ENV") or "test"
local Migrate = require('routines.Migrate')
local M = {}

-------------------------------------------------------------------------------
-- MIGRATE START
-------------------------------------------------------------------------------

function M:start()
  for i = 1, os.cores() do
    self:enqueue(tostring(i))
  end
end

-------------------------------------------------------------------------------
-- MIGRATE BEFORE
-------------------------------------------------------------------------------

function M:before()
  local ActiveRecord = require('arken.ActiveRecord')
  ActiveRecord.loadConfig{ number = self:number() }
end

-------------------------------------------------------------------------------
-- MIGRATE RUN
-------------------------------------------------------------------------------

function M:run()
  local migrate = Migrate.new()
  migrate:run()
end

return M
