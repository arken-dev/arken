local helper = require('arken.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:hiddenField("id", 1234)
  local html   = [[<input type="hidden" id="id" name="id" value="1234">]]
  assert( result == html, result )
end

return test
