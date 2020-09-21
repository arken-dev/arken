local rhea         = require 'rhea'
local json         = require 'arken.json'
local Migrate      = require 'rhea.Migrate'
local ActiveRecord = require 'arken.ActiveRecord'

local test = {}
local res  = {}

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
  local adapter = ActiveRecord.factoryAdapter()
  adapter:execute("DROP TABLE IF EXISTS schema_migration")
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  package.path = package.path .. ';' .. ARKEN_PATH .. '/util/?.lua'
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
  local adapter = ActiveRecord.factoryAdapter()
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
  local result = result[1]:simplified()
  assert( result:contains('run #') == true, result[1] )
  assert( result:contains('generate #') == true, result[1] )
end

test.should_1_output  = function()
  local params = {}
  params[0] = 'migrate:run'
  rhea.run(params)
  assert( #result == 3, json.encode(result) )
  assert( result[1] == '20170114133130 execute...', result[1] )
  assert( result[2] == '20170308223601.lua execute...', result[2] )
  assert( result[3] == 'migrations finished', result[3] )
end

test.should_output_two_runs  = function()
  local params = {}
  params[0] = 'migrate:run'
  rhea.run(params)
  result = {}
  rhea.run(params)
  assert( #result == 3, #result )
  assert( result[1] == '20170114133130 version ok', result[1] )
  assert( result[2] == '20170308223601.lua version ok', result[2] )
  assert( result[3] == 'migrations finished', result[3] )
end

return test
