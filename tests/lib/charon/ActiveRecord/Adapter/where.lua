local Adapter   = require('arken.ActiveRecord.Adapter')
local QDateTime = require('QDateTime')

local test = {}

test.should_return_with_order = function()
  local adapter = Adapter.new()
  local where   = adapter:where{ order = 'id' }
  assert( where == ' ORDER BY id', where )
end

test.should_return_with_limit = function()
  local adapter = Adapter.new()
  local where   = adapter:where{ limit = 100 }
  assert( where == ' LIMIT 100', where )
end

test.should_return_with_order_and_limit = function()
  local adapter = Adapter.new()
  local where   = adapter:where{ order = 'id', limit = 100 }
  assert( where == ' ORDER BY id LIMIT 100', where )
end

test.should_return_with_join = function()
  local adapter = Adapter.new()
  local where   = adapter:where{ join = { 'LEFT JOIN order ON customer.id = order.customer_id' } }
  assert( where == 'LEFT JOIN order ON customer.id = order.customer_id', where )
end

test.should_return_with_join_and_values = function()
  local adapter    = Adapter.new()
  local created_at = '2010/03/01'
  local join       = {
   'LEFT JOIN order ON customer.id = order.customer_id AND created_at > $created_at',
   created_at = created_at
  }
  local where      = adapter:where{ join = join }
  assert( where == "LEFT JOIN order ON customer.id = order.customer_id AND created_at > '2010/03/01'", where )
end

test.should_return_where_and_values = function()
  local adapter    = Adapter.new()
  local created_at = '2010/03/01'
  local where      = adapter:where{ where = 'created_at > $created_at', created_at = created_at }
  assert( where == " WHERE created_at > '2010/03/01'", where )
end

test.should_return_where_and_boolean = function()
  local adapter  = Adapter.new()
  local canceled = true
  local where    = adapter:where{ where = 'canceled = $canceled', canceled = canceled }
  assert( where == " WHERE canceled = 'true'", where )
end

test.should_return_where_and_number = function()
  local adapter = Adapter.new()
  local total   = 100.35
  local where   = adapter:where{ where = 'total > $total', total = total }
  assert( where == " WHERE total > 100.35", where )
end

test.should_return_where_and_userdata = function()
  local adapter = Adapter.new()
  local data    = QDateTime.currentDateTime()
  local where   = adapter:where{ where = 'data = $data', data = data }
  assert( where == string.format(" WHERE data = '%s'", data), where )
end

test.should_return_where_and_table_with_strings = function()
  local adapter = Adapter.new()
  local names   = {'John', 'Steve', 'Norman'}
  local where   = adapter:where{ where = 'name IN ($names)', names = names }
  assert( where == " WHERE name IN ('John','Steve','Norman')", where )
end

test.should_return_where_and_table_with_numbers = function()
  local adapter = Adapter.new()
  local values  = {11234.34, 950.55, 137.35}
  local where   = adapter:where{ where = 'values IN ($values)', values = values }
  assert( where == " WHERE values IN (11234.34,950.55,137.35)", where )
end

test.should_return_where_and_2_parameters = function()
  local adapter    = Adapter.new()
  local created_at = '2010/03/01'
  local customer_id = 1500
  local where      = adapter:where{ customer_id = customer_id, created_at = created_at }
  assert( where == " WHERE created_at = '2010/03/01' AND customer_id = 1500", where )
end

test.should_error_if_values_emptys_and_flag_true = function()
  local adapter     = Adapter.new()
  local created_at  = '2010/03/01'
  local customer_id = 1500
  local status, message = pcall(adapter.where, adapter, {}, true)
  assert( status == false )
  assert( message[1]:contains("parameters for find empty") == true, message )
end

test.should_return_where_with_boolean_value = function()
  local adapter = Adapter.new()
  local cancel  = true
  local where   = adapter:where{ cancel = cancel }
  assert( where == " WHERE cancel = 'true'", where )
end

test.should_return_where_with_NULL = function()
  local adapter = Adapter.new()
  local flag    = 'NULL'
  local where   = adapter:where{ flag = flag }
  assert( where == " WHERE flag IS NULL", where )
end

test.should_return_where_with_NOT_NULL = function()
  local adapter = Adapter.new()
  local flag    = 'NOT NULL'
  local where   = adapter:where{ flag = flag }
  assert( where == " WHERE flag IS NOT NULL", where )
end

test.should_return_where_with_userdata = function()
  local QDateTime = require('QDateTime')
  local adapter   = Adapter.new()
  local date      = QDateTime.currentDateTime()
  local where     = adapter:where{ date = date }
  local result    = string.format(" WHERE date = '%s'", date:__tostring())
  assert( where == result, where )
end

test.should_return_where_with_table = function()
  local adapter   = Adapter.new()
  local values    = {12, 29, 33}
  local where     = adapter:where{ id = values }
  local result    = ' WHERE id IN (12,29,33)'
  assert( where == result, where )
end

return test
