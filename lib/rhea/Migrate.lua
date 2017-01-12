require 'charon.ActiveRecord'

local Migrate = Class.new("Migrate")

Migrate.help = {}

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
  local glob = os.glob("db/migrate", ".*(lua$|sql$)", true)
  local list = {}
  for i = 1, glob:size() do
    table.insert(list, glob:at(i))
  end
  table.sort(list)

  for _, file_name in ipairs(list) do
    local index1  = file_name:lastIndexOf('/') + 1
    local index2  = file_name:indexOf('_') - index1
    local version = file_name:mid(index1, index2)

    if self.list[version] then
        print(string.format("%s version ok", version))
    else
      local sql = os.read(file_name)
      --print(sql)
      ActiveRecord.adapter():execute(sql)
      ActiveRecord.adapter():execute(string.format([[INSERT INTO schema_migration VALUES ('%s')]], version))
    end
  end
  print("migrations finished")
end

Migrate.contract('run')

return Migrate
