local helper     = require 'arken.Helper'
local dispatcher = require 'arken.dispatcher'
local test       = {}

test.should_return_html = function()
  local result = helper:stylesheet("main")
  local html   = [[<link href="/stylesheets/main.css?0" media="screen" rel="stylesheet" type="text/css" />]]
  assert( result == html, result )
end

test.should_return_html_with_dispatcher_prefix = function()
  dispatcher.prefix = '/app'
  local result = helper:stylesheet("main")
  dispatcher.prefix = ''
  local html   = [[<link href="/app/stylesheets/main.css?0" media="screen" rel="stylesheet" type="text/css" />]]
  assert( result == html, result )
end

return test
