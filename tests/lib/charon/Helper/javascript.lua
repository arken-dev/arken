local helper     = require('charon.Helper')
local dispatcher = require('charon.dispatcher')
local test       = {}

test.before = function()
  dispatcher.prefix = ""
  dispatcher.controller = nil
  dispatcher.controller_name = nil
  dispatcher.action = nil
end

test.should_return_html = function()
  local result = helper:javascript("main")
  local html   = [[<script src="/javascripts/main.js?0" type="text/javascript"></script>]]
  assert( result == html, result )
end

test.should_return_html_with_dispatcher_prefix = function()
  dispatcher.prefix = '/app'
  local result = helper:javascript("main")
  dispatcher.prefix = ''
  local html   = [[<script src="/app/javascripts/main.js?0" type="text/javascript"></script>]]
  assert( result == html, result )
end

return test
