-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local colorize = {}

colorize.black     = '0;30'
colorize.red       = '0;31'
colorize.red_light = '1;31'
colorize.green     = '0;32'
colorize.yellow    = '0;33'
colorize.blue      = '0;34'
colorize.magenta   = '0;35'
colorize.cyan      = '0;36'
colorize.white     = '0;37'

colorize.format = function(text, color)
  color = colorize[color]
  if os.name() == 'windows' then
    return text
  else
    return string.char(27)..'[' .. color .. 'm' .. text .. string.char(27) ..'[0m'
  end
end

return colorize
