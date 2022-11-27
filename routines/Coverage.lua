local worker = require('arken.concurrent.worker')
local mvm    = require('arken.mvm')
mvm.env('test')

local Coverage = Class.new("routines.Coverage", "arken.routine.Base")

-------------------------------------------------------------------------------
-- MODELS
-------------------------------------------------------------------------------

Coverage.help.models = [[
  coverage models
]]

function Coverage:models()
  worker.start('workers.coverage.models');
  mvm.wait();
end

-------------------------------------------------------------------------------
-- MIGRATE
-------------------------------------------------------------------------------

Coverage.help.migrate = [[
  migrate all databases with TRITON_NUMBER
]]

function Coverage:migrate()
  worker.start('workers.migrate');
  worker.wait();
end

return Coverage
