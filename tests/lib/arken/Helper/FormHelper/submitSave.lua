local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_html = function()
  request = {}
  local form   = FormHelper.new()
  local result = form:submitSave()
  local html   = [[<input type="image" src="/images/icons/botao_gravar.png?1453303287" style="border: 0px" >]]
  assert( result  == html, result )
end

return test
