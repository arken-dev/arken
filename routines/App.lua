local mvm      = require('arken.mvm')
local empty    = require('arken.empty')
local template = require('arken.template')
local json     = require('arken.jsonp')
local Class    = require('arken.oop.Class')


local App   = Class.new("App")
App.help    = {}
App.output  = print

-------------------------------------------------------------------------------
-- CREATE
-------------------------------------------------------------------------------

App.help.create = [[
  create app in dir
]]

function App:createValidate()
  if empty(self:params()[1]) then
    error('inform name')
  end
end

function App:create()
  local params  = self:params()
  local name    = params[1]
  local dirName = os.pwd() .. '/' .. name
  if os.exists(dirName) then
    error(dirName .. ' exists')
  end
  if params.cpp == nil and params.web == nil then
    error('inform --cpp or --web')
  end

  if params.cpp then
    os.mkdir( name )
    os.mkdir( string.format('%s/src', name) )
    os.mkdir( string.format('%s/bin', name) )
    os.mkdir( string.format('%s/build', name) )
    local path   = string.format("%s/routines/templates/cmake/app.tpl", mvm.path())
    local buffer = template.execute(path, { name = name })

    local file = io.open(dirName .. '/CMakeLists.txt', 'w')
    file:write(buffer)
    file:close()
  end

  if params.web then
    local path = os.dirpath(os.executablePath())
    os.copy(path .. '/../skel', dirName)
    for fileName in os.find(dirName, ".*", true):each() do
      if not fileName:endsWith('.') and not fileName:endsWith('..') then
        App.output(fileName)
      end
    end

    local fileName = dirName .. '/config/active_record.json'
    local data     = json.decode(os.read(fileName))
    local database = params.database or "arkenapp"

    if params.postgres then
      data.development.adapter = "arken.ActiveRecord.PostgresAdapter"
      data.production.adapter = "arken.ActiveRecord.PostgresAdapter"
      data.test.adapter = "arken.ActiveRecord.PostgresAdapter"
    end

    if params.mysql then
      data.development.adapter = "arken.ActiveRecord.MysqlAdapter"
      data.production.adapter = "arken.ActiveRecord.MysqlAdapter"
      data.test.adapter = "arken.ActiveRecord.MysqlAdapter"
    end

    if params.sqlite then
      data.development.adapter = "arken.ActiveRecord.SqliteAdapter"
      data.production.adapter = "arken.ActiveRecord.SqliteAdapter"
      data.test.adapter = "arken.ActiveRecord.SqliteAdapter"
    end

    data.test.database = database:append("_test")

    local file = io.open(fileName, "w")
    file:write(json.pretty(data))
    file:close()
  end
end

App.contract('create')

return App
