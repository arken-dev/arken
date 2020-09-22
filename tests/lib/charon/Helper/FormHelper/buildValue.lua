local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_nil_if_data_not_exists = function()
  local form = FormHelper.new()
  assert( form:buildValue('id') == '' )
end

test.should_return_value_of_data_field = function()
  local data = { id = 1234 }
  local form = FormHelper.new{ data = data }
  assert( form:buildValue('id') == 1234 )
end

test.should_return_value_of_data_read = function()
  local data = { id = '1234', read = function(data, field) return tonumber(data[field]) end }
  local form = FormHelper.new{ data = data }
  assert( form:buildValue('id') == 1234 , form:buildValue('id') )
end

return test
