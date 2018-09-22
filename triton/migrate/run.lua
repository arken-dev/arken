CHARON_ENV = os.getenv("CHARON_ENV") or "test"
local Migrate = require('rhea.Migrate')
local M = {}

-------------------------------------------------------------------------------
-- START
-------------------------------------------------------------------------------

function M.start()
  local JSON = require('charon.json')
  local file = APP_PATH .. '/config/triton.json'
  local raw  = os.read(file)
  if os.exists(file) then
    local config  = JSON.decode(raw)
    local threads = config.threads
    for i = 1, threads do
      triton.enqueue(tostring(i))
    end
  else
    error(fileName .. ' not exists')
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
