local ActiveRecord = require('charon.ActiveRecord')
local ActiveRecord_SqliteMemoryAdapter = Class.new("ActiveRecord.SqliteMemoryAdapter", "ActiveRecord.SqliteAdapter")
local json = require('charon.json')

ActiveRecord.schema   = ActiveRecord.schema   or "db/schema"
ActiveRecord.fixtures = ActiveRecord.fixtures or "db/fixtures"

-------------------------------------------------------------------------------
-- LOADER
-------------------------------------------------------------------------------

ActiveRecord_SqliteMemoryAdapter.loader = function()
  local init = os.microtime()
  local list = os.glob(ActiveRecord.schema, "json$")
  for i = 1, list:size() do
    local fileJson  = list:at(i)
    local tableName = fileJson:chop(5):mid(fileJson:lastIndexOf('/')+1, -1)
    local adapter   = ActiveRecord.factoryAdapter{ tableName = tableName }
    adapter:execute( adapter:schema() )
    local jsonList = os.glob(ActiveRecord.fixtures .. '/' .. adapter.tableName, true)
    for i = 1, jsonList:size() do
      local fileJson = jsonList:at(i)
      if not (fileJson:endsWith('.') or fileJson:endsWith('..')) then
        local data = json.decode(os.read(fileJson))
        local sql  = adapter:insert(data)
        --print(sql)
        adapter:execute(sql)
      end
    end
  end
  -- print(string.format("loader in %2f", os.microtime() - init))
end

return ActiveRecord_SqliteMemoryAdapter
