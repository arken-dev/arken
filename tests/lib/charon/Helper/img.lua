local helper     = require 'charon.Helper'
local dispatcher = require 'charon.dispatcher'
local test       = {}

test.should_return_html = function()
  local result = helper:img("main.png")
  local html   = [[<img src="/images/main.png?0" title="main" />]]
  assert( result == html, result )
end

test.should_return_html_with_title = function()
  local result = helper:img("main.png", { title = 'my image' } )
  local html   = [[<img src="/images/main.png?0" title="my image" />]]
  assert( result == html, result )
end

test.should_return_html_with_prefix = function()
  dispatcher.prefix = '/app'
  local result = helper:img("main.png", { title = 'my image' } )
  dispatcher.prefix = ''
  local html   = [[<img src="/app/images/main.png?0" title="my image" />]]
  assert( result == html, result )
end


return test
