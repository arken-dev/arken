require 'arken.ActiveRecord'

local Lock     = require("arken.concurrent.Lock")
local lock     = Lock.new("routines.Migrate#print")
local Migrate  = Class.new("routines.Migrate")
Migrate.help   = {}
Migrate.dir    = nil
Migrate.output = function(value)
  lock:enable()
  print(value)
  lock:disable()
end

-------------------------------------------------------------------------------
-- DIR
-------------------------------------------------------------------------------

if os.exists('migrate') then
  -- default dir in generate migrate
  Migrate.dir = "migrate"
end

if os.exists('db/migrate') then
  Migrate.dir = "db/migrate"
end

-------------------------------------------------------------------------------
-- RUN
-------------------------------------------------------------------------------

Migrate.help.run = [[
  execute all migrates
]]

function Migrate:runPrepare()
  self.list = ActiveRecord.adapter():prepareMigration()
end

function Migrate:run()
  local files = os.find(Migrate.dir, ".*(lua$|sql$)")
  local list  = {}
  for i = 1, files:size() do
    table.insert(list, files:at(i))
  end
  table.sort(list)

  for _, fileName in ipairs(list) do
    local index   = fileName:lastIndexOf('/') + 1
    local version = fileName:mid(index, 14)
    local suffix  = fileName:suffix(".")

    if self.list[version] then
      Migrate.output(string.format("%s version ok", version))
    else
      Migrate.output(string.format("%s execute...", version))

      if suffix == 'sql' then
        local sql = os.read(fileName)
        ActiveRecord.adapter():execute(sql)
      end

      if suffix == 'lua' then
        dofile(fileName)
      end

      local sql = [[INSERT INTO schema_migration VALUES ('%s')]]
      ActiveRecord.adapter():execute(string.format(sql, version))
    end
  end
  Migrate.output("migrations finished")
end

Migrate.contract('run')

-------------------------------------------------------------------------------
-- GENERATE
-------------------------------------------------------------------------------

Migrate.help.generate = [[
  generate migrate
]]

function Migrate:generate()
  local Time      = require('arken.chrono.Time')
  local params    = self:params()
  local timestamp = Migrate.timestamp or Time.now():strftime('%Y%m%d%H%M%S')
  local name      = tostring(params[1]):underscore()
  if name == 'nil' then
    error('arg for migrate name')
  end
  local ext = 'lua'
  if params.sql then
    ext = 'sql'
  end
  if Migrate.dir == nil then
    os.mkpath('migrate')
  end
  local fileName = string.format(Migrate.dir .. '/%s_%s.%s', timestamp, name, ext)
  Migrate.output('create ' .. fileName )
  local file = io.open(fileName, 'w')
  file:write(string.format('-- migrate %s_%s.%s', timestamp, name, ext))
  file:close()
end

return Migrate
