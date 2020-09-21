local helper     = require 'arken.Helper'
local dispatcher = require 'arken.dispatcher'
local test       = {}

test.should_return_html = function()
  local result = helper:imgPath("main.png")
  local html   = [[/images/main.png?0]]
  assert( result == html, result )
end

test.should_return_html_with_prefix = function()
  dispatcher.prefix = '/app'
  local result = helper:imgPath("main.png")
  dispatcher.prefix = ''
  local html   = [[/app/images/main.png?0]]
  assert( result == html, result )
end

return test
