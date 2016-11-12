local test = {}
local Person = Class.new("Person", "ActiveRecord")
local uuid = ""

test.before = function()
  uuid = os.uuid():replace('-', '_')
  Person.table_name = 'person_' .. uuid
  Person[Person.adapter_instance] = nil
  Person.adapter():execute(string.format([[
  CREATE TABLE person_%s (
    id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  )]], uuid))
end

test.after = function()
  Person.adapter():connect():exec(string.format([[DROP TABLE person_%s]], uuid))
end

test.should_return_record_stored = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local result = Person.find{ name = "Chris Weidman" }
  assert( result.name == 'Chris Weidman', json.encode(result) )
end

test.should_return_instance_by_primary_key = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  local result = Person.find{ id = p.id }

  assert( p == result, tostring(p) .. ' ' .. tostring(result) )
end

test.should_return_one_id = function()
  local p = Person.new()
  p.name = "Junior Cigano"
  p:save()
  local result = Person.find{ name = "Junior Cigano" }

  assert( p.id == result.id, 'id diferentes: ' .. p.id .. ' ' .. result.id )
end


test.should_return_instance_by_other_attribute = function()
  local p = Person.new()
  p.name = "Junior Cigano"
  p:save()
  local result = Person.find{ name = "Junior Cigano" }

  assert( p == result, tostring(p) .. ' ' .. tostring(result) )
end

return test
