local mimetype = require('arken.net.mime-type')
local test = {}

test.should_contains_text_mime_type = function()
  assert( mimetype.text == 'text/plain', mimetype.text )
end

test.should_contains_html_mime_type = function()
  assert( mimetype.html == 'text/html', mimetype.html )
end

return test
