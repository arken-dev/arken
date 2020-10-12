local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS person (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT, total REAL
  )]]
  Person.adapter():execute(sql)
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_return_cursor = function()
  ActiveRecord.query_prefix = ARKEN_PATH .. '/util/'
  Person.where = function(params)
    return params
  end
  local record = Person.query( 'all', {} )
  assert( tostring(record):contains('cursor') == true, tostring(record) )
end

test.should_error_if_file_not_exists = function()
  ActiveRecord.query_prefix = nil

  Person.where = function(params)
    return params
  end

  local status, message = pcall(Person.query, 'notexists', {} )
  assert( status == false )
  assert( message:contains('query/person/notexists.sql file not exists') == true , message )
end

test.should_query_with_binding = function()
  ActiveRecord.query_prefix = ARKEN_PATH .. '/util/'

  Person.where = function(params)
    return params
  end

  local sql = Person.adapter():sql('all_with_binding', { binding = { user_id = 153 } })
  assert( sql == 'SELECT person.* FROM person LEFT JOIN orders ON orders.person_id = person.id AND orders.user_id = 153\n', sql )  
end

test.should_return_error_if_where_method_return_nil = function()
  ActiveRecord.query_prefix = ARKEN_PATH .. '/util/'
  Person.where = function(params)
    return nil
  end
  local status, message = pcall(Person.query, 'all', {} )
  assert( status == false )
  assert( message:contains( 'Person#where not return valid query' ) == true, message )
end

return test
