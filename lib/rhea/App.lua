local json  = require('charon.jsonp')
local Class = require('charon.oop.Class')
local App   = Class.new("App")
App.help    = {}
App.output  = print

-------------------------------------------------------------------------------
-- CREATE
-------------------------------------------------------------------------------

App.help.create = [[
  create app in dir
]]

function App:create(params)
  local dirName = os.pwd() .. '/' .. params[1]
  if os.exists(dirName) then
    error(dirName .. ' exists')
  else
    os.copy(CHARON_PATH .. '/skel', dirName)
    for fileName in os.glob(dirName, true):each() do
      if not fileName:endsWith('.') and not fileName:endsWith('..') then
        App.output(fileName)
      end
    end

    local fileName = dirName .. '/config/active_record.json'
    local data     = json.decode(os.read(fileName))
    local database = params.database or "charonapp"

    if params.postgres then
      data.development.adapter = "charon.ActiveRecord.PostgresAdapter"
      data.production.adapter = "charon.ActiveRecord.PostgresAdapter"
      data.test.adapter = "charon.ActiveRecord.PostgresAdapter"
    end

    if params.mysql then
      data.development.adapter = "charon.ActiveRecord.MysqlAdapter"
      data.production.adapter = "charon.ActiveRecord.MysqlAdapter"
      data.test.adapter = "charon.ActiveRecord.MysqlAdapter"
    end

    if params.sqlite then
      data.development.adapter = "charon.ActiveRecord.SqliteAdapter"
      data.production.adapter = "charon.ActiveRecord.SqliteAdapter"
      data.test.adapter = "charon.ActiveRecord.SqliteAdapter"
    end

    data.test.database = database:append("_test")

    local file = io.open(fileName, "w")
    file:write(json.pretty(data))
    file:close()
  end
end

return App
