local Adapter = require('ActiveRecord').loadAdapter()

local test = {}

test.before = function()
  Adapter:execute([[
  CREATE TABLE active_record (
    id INTEGER PRIMARY KEY NOT NULL, content, contador INT, total REAL, observacao TEXT, sub_total NUMERIC,
    nome varchar(250)
  )]])
end

test.after = function()
  Adapter:execute([[DROP TABLE active_record]])
end

test.should_return_table = function()
  local columns = Adapter:columns()
  assert(type(columns) == 'table')
end

test.should_return_type = function()
  local columns = Adapter:columns()
  assert(columns.id.format == 'number', json.encode(columns.id))
  assert(columns.content.format == 'string', json.encode(columns.content))
  assert(columns.total.format == 'number', json.encode(columns.total))
  assert(columns.observacao.format == 'string', json.encode(columns.observacao))
end

test.should_return_not_null = function()
  local columns = Adapter:columns()
  assert(columns.id.format == 'number', json.encode(columns.id))
  assert(columns.id.not_null == true, json.encode(columns.id))
end

return test
