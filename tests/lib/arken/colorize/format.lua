local colorize = require('arken.colorize')
local test = {}

test.should_return_text_without_color_if_windows = function()
  colorize.osName = 'windows'
  assert( colorize.format('my text', 'green') == 'my text', colorize.format('my text', 'green') )
  colorize.osName = os.name()
end

test.should_return_text_without_color_if_not_windows = function()
  local result = string.char(27)..'[' .. '0;32' .. 'm' .. 'my text' .. string.char(27) ..'[0m'
  assert( colorize.format('my text', 'green') == result, colorize.format('my text', 'green') )
end

return test
