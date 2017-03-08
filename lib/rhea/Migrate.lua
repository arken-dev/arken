require 'charon.ActiveRecord'

local Migrate = Class.new("Migrate")

Migrate.help   = {}
Migrate.dir    = "db/migrate"
Migrate.output = print

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
  local glob = os.glob(Migrate.dir, ".*(lua$|sql$)", true)
  local list = {}
  for i = 1, glob:size() do
    table.insert(list, glob:at(i))
  end
  table.sort(list)

  for _, fileName in ipairs(list) do
    local index1  = fileName:lastIndexOf('/') + 1
    local index2  = fileName:indexOf('_') - index1
    local version = fileName:mid(index1, index2)

    if self.list[version] then
        Migrate.output(string.format("%s version ok", version))
    else

      if fileName:suffix() == 'sql' then
        local sql = os.read(fileName)
        ActiveRecord.adapter():execute(sql)
        local sql = [[INSERT INTO schema_migration VALUES ('%s')]]
        ActiveRecord.adapter():execute(string.format(sql, version))
      end

      if fileName:suffix() == 'lua' then
        dofile(fileName)
        local sql = [[INSERT INTO schema_migration VALUES ('%s')]]
        ActiveRecord.adapter():execute(string.format(sql, version))
      end

    end
  end
  Migrate.output("migrations finished")
end

Migrate.contract('run')

return Migrate
