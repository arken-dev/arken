local triton = require('arken.concurrent.triton')
local mvm    = require('arken.mvm')

local Coverage = Class.new("routines.Coverage")

Coverage.help   = {}

-------------------------------------------------------------------------------
-- MODELS
-------------------------------------------------------------------------------

Coverage.help.models = [[
  coverage models
]]

function Coverage:models()
  triton.start('triton.coverage.models');
  triton.wait();
end

-------------------------------------------------------------------------------
-- MIGRATE
-------------------------------------------------------------------------------

Coverage.help.migrate = [[
  migrate all databases with TRITON_NUMBER
]]

function Coverage:migrate()
  triton.start('triton.migrate');
  triton.wait();
end

return Coverage
