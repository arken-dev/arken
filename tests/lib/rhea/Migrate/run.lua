local rhea = require 'rhea'
local json = require 'charon.json'
local test = {}
local res  = {}
local ActiveRecord = require('charon.ActiveRecord')
local Migrate = require('rhea.Migrate')

Migrate.dir = "util/db/migrate"
-------------------------------------------------------------------------------
-- OUTPUT TEST

local result = {}

function output(value)
  table.insert(result, value)
end

Migrate.output = output
rhea.output = output

test.before = function()
  result = {}
end

test.after = function()
  local adapter = ActiveRecord.loadAdapter()
  adapter:execute("DROP TABLE IF EXISTS schema_migration")
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  package.path = package.path .. ';' .. CHARON_PATH .. '/util/?.lua'
  rhea.path = package.path
end

test.afterAll = function()
  ActiveRecord.config = nil
  package.path = package.path:mid(1, rhea.path:lastIndexOf(';')-1)
end

-------------------------------------------------------------------------------
-- MODULE TEST

test.should_insert_timestamp_in_schema_migrations = function()
  local params = {}
  params[0] = 'migrate:run'
  rhea.run(params)
  local adapter = ActiveRecord.loadAdapter()
  local cursor  = adapter:execute('SELECT * FROM schema_migration')
  local row     = cursor:fetch('a', {})
  assert( type(cursor), type(cursor) )
  assert( row == '20170114133130', json.encode(row) )
  cursor:close()
end

test.should_migrate_helper = function()
  local params = {}
  params[0] = 'migrate'
  params[1] = '--help'
  rhea.run(params)
  assert( result[1] == 'run # execute all migrates\n', result[1] )
end

test.should_1_output  = function()
  local params = {}
  params[0] = 'migrate:run'
  rhea.run(params)
  assert( #result == 1, #result )
  assert( result[1] == 'migrations finished', result[1] )
end

test.should_output_two_runs  = function()
  local params = {}
  params[0] = 'migrate:run'
  rhea.run(params)
  result = {}
  rhea.run(params)
  assert( #result == 2, #result )
  assert( result[1] == '20170114133130 version ok', result[1] )
  assert( result[2] == 'migrations finished', result[2] )
end

return test
