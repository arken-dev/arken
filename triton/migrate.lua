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
-- TRITON RUN
-------------------------------------------------------------------------------

function M.run(number)
  local migrate = Migrate.new()
  migrate:run()
end

-------------------------------------------------------------------------------
-- TRITON STOP
-------------------------------------------------------------------------------

function M.stop()
end

return M
