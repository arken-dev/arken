local helper     = require('arken.Helper')
local dispatcher = require('arken.dispatcher')
local test       = {}

test.before = function()
  dispatcher.prefix = ""
  dispatcher.controller = nil
  dispatcher.controller_name = nil
  dispatcher.action = nil
  -- TODO
  helper.controller = { prefix = '' }
end

test.should_return_html = function()
  local result = helper:javascript("main")
  local html   = [[<script src="/javascripts/main.js?0" type="text/javascript"></script>]]
  assert( result == html, result .. '\n\n' .. result)
end

test.should_return_html_with_dispatcher_prefix = function()
  dispatcher.prefix = '/app'
  local result = helper:javascript("main")
  dispatcher.prefix = ''
  local html   = [[<script src="/javascripts/main.js?0" type="text/javascript"></script>]]
  assert( result == html, result .. '\n\n' .. html)
end

return test
