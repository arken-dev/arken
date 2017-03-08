local test    = {}
local json    = require('charon.json')
local Class   = require('charon.oop.Class')
local Adapter = require("charon.ActiveRecord.Adapter")
local Person  = Class.new("Person", "ActiveRecord")

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

test.should_validate_presence = function()
  Person.saveValidate = function(self)
    self:validate{ "presence",
      column  = 'name',
      message = 'name is invalid'
    }
  end
  Person.contract('save')

  local person = Person.new()
  person.name = ""
  local status, message = person:pcall('save')
  assert(status == false)
  assert(message.name == 'name is invalid', message.name)
end

test.should_validate_boolean = function()
  Person.saveValidate = function(self)
    self:validate{ "boolean",
      column  = 'cancel',
      message = 'name is not a boolean'
    }
  end
  Person.contract('save')

  local person = Person.new()
  person.name = ""
  local status, message = person:pcall('save')
  assert(status == false)
  assert(message.cancel == 'name is not a boolean', json.encode(message))
end

test.should_validate_length_min = function()
  Person.saveValidate = function(self)
    self:validate{ "length",
      min     = 6,
      column  = 'name',
      message = 'name is minor than 5'
    }
  end
  Person.contract('save')

  local person = Person.new()
  person.name = "Chris"
  local status, message = person:pcall('save')
  assert(status == false)
  assert(message.name == 'name is minor than 5', json.encode(message))
  assert( #person.errors == 0 )
end

test.should_validate_length_with_empty_value = function()
  Person.saveValidate = function(self)
    self:validate{ "length",
      min     = 6,
      column  = 'name',
      message = 'name is minor than 5'
    }
  end
  Person.contract('save')

  local person = Person.new()
  person.name = nil
  local status, message = person:pcall('save')
  assert(status == false)
  assert(message.name == 'name is minor than 5', json.encode(message))
  assert( #person.errors == 0 )
end


test.should_validate_length_max = function()
  Person.saveValidate = function(self)
    self:validate{ "length",
      max     = 255,
      column  = 'name',
      message = 'name is major than 255'
    }
  end
  Person.contract('save')

  local person = Person.new()
  person.name = string.rep("a", 300)
  local status, message = person:pcall('save')
  assert(status == false)
  assert(message.name == 'name is major than 255', json.encode(message))
  assert( #person.errors == 0 )
end

return test
