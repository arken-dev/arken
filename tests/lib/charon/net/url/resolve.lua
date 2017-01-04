local json = require('charon.json')
local url  = require('charon.net.url')
local test = {}

test.should_return_table_form_url = function()
  local url1   = "http://username:password@hostname/path?arg=value#anchor"
  local url2   = "http://username:password@hostname/path?arg=1234"
  local result = url.parse(url1)
  assert( result:resolve("?arg=1234"):build() == url2, result:resolve("?arg=1234"):build() )
end

test.should_return_table_change_params = function()
  local result = url.parse("http://username:password@hostname:2345/path?arg=value#anchor")
  assert( result:build() == "http://username:password@hostname:2345/path?arg=value#anchor", result:build() )
end

test.should_return_new_url_if_scheme_if_different = function()
  local url1   = "http://username:password@hostname/path?arg=value#anchor"
  local url2   = "http://username:password@hostname/path?arg=1234"
  local result = url.parse(url1)
  assert( result:resolve("https://newlink"):build() == "https://newlink" )
end

test.should_return_without_query_string = function()
  local url1   = "http://newlink/path"
  local url2   = "http://newlink/path?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("path?id=1234"):build() == url2 )
end

test.should_return_without_path_and_only_querystring = function()
  local url1   = "http://newlink/path"
  local url2   = "http://newlink/?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("/?id=1234"):build() == url2, result:resolve("/?id=1234"):build() )
end

test.should_return_domain = function()
  local url1   = "http://newlink.org?id=1234"
  local url2   = "http://newlink.org/mypath?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("/mypath"):build() == url2, result:resolve("/mypath"):build() )
end

test.should_return_with_relative_path = function()
  local url1   = "http://newlink.org/path?id=1234"
  local url2   = "http://newlink.org/mypath?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("./mypath"):build() == url2, result:resolve("/mypath"):build() )
end

test.should_return_remove_path = function()
  local url1   = "http://newlink.org/path?id=1234"
  local url2   = "http://newlink.org/?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("./"):build() == url2, result:resolve("./"):build() )
end

test.should_return_dot_path_example_1 = function()
  local url1   = "http://newlink.org/../../?id=1234"
  local url2   = "http://newlink.org/path?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("path"):build() == url2, result:resolve("path"):build() )
end

test.should_return_dot_path_example_3 = function()
  local url1   = "http://newlink.org/../../mycustom?id=1234"
  local url2   = "http://newlink.org/mycustom?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("../../mycustom"):build() == url2, result:resolve("../../mycustom"):build() )
end

test.should_return_dot_path_example_4 = function()
  local url1   = "http://newlink.org/mycustom?id=1234"
  local url2   = "http://newlink.org/?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("../.."):build() == url2, result:resolve("../.."):build() )
end

test.should_return_dot_path_example_5 = function()
  local url1   = "http://newlink.org/../?id=1234"
  local url2   = "http://newlink.org/..?id=1234"
  local result = url.parse(url1)
  assert( result:resolve("../../.."):build() == url2, result:resolve("../../.."):build() )
end

return test
