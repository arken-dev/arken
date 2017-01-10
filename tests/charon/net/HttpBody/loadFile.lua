local HttpBody = require('charon.net.HttpBody')

local test = {}

test.should_load_file_and_return_content = function()
  local body = HttpBody.loadFile('util/hello-world.txt')
  assert(body:read() == 'hello world\n', body:read())
  assert(body:size() == 12, body:size())
end

test.should_return_nil_after_release = function()
  local body = HttpBody.loadFile('util/hello-world.txt')
  assert(body:read() == 'hello world\n', body:read())
  assert(body:size() == 12, body:size())
  body:release()
  assert(body:read() == '', body:read())
  assert(body:size() == 0, body:size())
end

return test
