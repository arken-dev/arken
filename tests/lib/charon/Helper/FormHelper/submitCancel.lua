local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_html = function()
  request = {}
  local form   = FormHelper.new({ controller = {} })
  local result = form:submitCancel()
  local html   = [[<a href="/index/cancel" data-remote="true" ><img src="/images/icons/botao_cancelar.png?0" title="icons/botao_cancelar" /></a>]]
  assert( result  == html, result )
end

return test
