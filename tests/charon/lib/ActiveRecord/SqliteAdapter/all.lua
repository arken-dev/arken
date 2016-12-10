local test = {}
local Person = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE person (
    id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
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
  Person.adapter():execute("DROP TABLE person")
  ActiveRecord.config = nil
end

test.should_return_record_stored = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local result = Person.all{ name = "Chris Weidman" }
  assert( result[1].name == 'Chris Weidman', json.encode(result) )
end

test.should_return_instance_by_primary_key = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  local result = Person.all{ id = p.id }

  assert( p == result[1], tostring(p) .. ' ' .. tostring(result) )
end

test.should_return_one_id = function()
  local p = Person.new()
  p.name = "Junior Cigano"
  p:save()
  local result = Person.all{ name = "Junior Cigano" }

  assert( p.id == result[1].id, string.format('%i %i', p.id, result[1].id) )
end


test.should_return_instance_by_other_attribute = function()
  local p = Person.new()
  p.name = "Junior Cigano"
  p:save()
  local result = Person.all{ name = "Junior Cigano" }

  assert( p == result[1], tostring(p) .. ' ' .. tostring(result[1]) )
end

return test
