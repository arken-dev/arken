ARKEN_ENV = os.getenv("ARKEN_ENV") or "test"
local Migrate = require('routines.Migrate')
local M = {}

-------------------------------------------------------------------------------
-- START
-------------------------------------------------------------------------------

function M.start(triton)
  for i = 1, os.cores() do
    triton:enqueue(tostring(i))
  end
end

-------------------------------------------------------------------------------
-- TRITON BEFORE
-------------------------------------------------------------------------------

function M.before(triton)
  local ActiveRecord = require('arken.ActiveRecord')
  ActiveRecord.loadConfig{ triton = triton:number() }
end

-------------------------------------------------------------------------------
-- TRITON RUN
-------------------------------------------------------------------------------

function M.run(triton)
  local migrate = Migrate.new()
  migrate:run()
end

-------------------------------------------------------------------------------
-- TRITON AFTER
-------------------------------------------------------------------------------

function M.after(triton)
end

-------------------------------------------------------------------------------
-- TRITON STOP
-------------------------------------------------------------------------------

function M.stop()
end

return M
