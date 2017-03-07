local helper     = require('charon.Helper')
local dispatcher = require('charon.dispatcher')
local test       = {}

test.before = function()
  dispatcher.prefix = ""
  helper.controller = nil
  helper.controllerName = nil
  helper.action = nil
end

test.should_return_html_with_minimal_params = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' } }
  local html   = [[<a href="/index/index">hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_remote = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, remote = true }
  local html   = [[<a href="/index/index" data-remote="true" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_after = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, after = "alert('here !')"}
  local html   = [[<a href="/index/index" data-after="alert('here !')" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_click = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, click = "alert('here !')"}
  local html   = [[<a href="/index/index" onclick="alert('here !')" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_update = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, update = "alert('here !')" }
  local html   = [[<a href="/index/index" data-update="alert('here !')" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_confirm = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, confirm = "alert('here !')" }
  local html   = [[<a href="/index/index" data-confirm="alert('here !')" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_json = function()
  local url    = { action = 'index' }
  local result = helper:link{ text = 'hello', url = url, remote = true, with = [[{ id: "name" }]] }
  local html   = [[<a href="/index/index" data-remote="true"  data-with="{ id: \"name\" }" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_condition = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, condition = "alert('here !')" }
  local html   = [[<a href="/index/index" data-condition="alert('here !')" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_style = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, style = "background-color: red" }
  local html   = [[<a href="/index/index" style="background-color: red" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_class = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, class = "myclass" }
  local html   = [[<a href="/index/index" class="myclass" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_title = function()
  local result = helper:link{ text = 'hello', url = { action = 'index' }, title = "click here" }
  local html   = [[<a href="/index/index" title="click here" >hello</a>]]
  assert( result == html, result )
end

test.should_return_html_with_title = function()
  local result = helper:link{ img = 'hello.png', url = { action = 'index' }, title = "click here" }
  local html   = [[<a href="/index/index" title="click here" ><img src="/images/hello.png?0" title="click here" /></a>]]
  assert( result == html, result )
end

return test
