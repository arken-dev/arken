require "CHttpParser"

local test = {}

test.should_return_nil_with_queryString = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  assert(parser:queryString() == nil)
end

test.should_return_nil_with_fragment = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  assert(parser:fragment() == nil)
end

test.should_return_nil_with_requestPath = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  assert(parser:requestPath() == '/app/fornecedor/filtrar/indexPerform', parser:requestPath())
end

test.should_return_nil_with_requestMethod = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  assert(parser:requestMethod() == 'POST', parser:requestMethod())
end

test.should_return_nil_with_requestUri = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  assert(parser:requestUri() == '/app/fornecedor/filtrar/indexPerform', parser:requestUri())
end

test.should_return_nil_with_httpVersion = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  assert(parser:httpVersion() == 'HTTP/1.0', parser:httpVersion())
end

test.should_return_nil_with_headerDone = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  --assert(parser:headerDone():contains([[Content-Disposition: form-data; name="id"]]))
  assert(parser:headerDone():startsWith([[-----------------------------152822217111798973981392119654]]), parser:headerDone())
end

test.should_return_form_data_with_contentType = function()
  local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
  local parser = CHttpParser.new(header)
  assert(parser:field('Content-Type'):startsWith('multipart/form-data;'), parser:field('Content-Type'))
end

return test
