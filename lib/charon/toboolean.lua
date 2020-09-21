-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.
local convertion = {
  [true]    = true,
  ['true']  = true,
  ['t']     = true,
  ['1']     = true,
  [1]       = true,
  [false]   = false,
  ['false'] = false,
  ['f']     = false,
  ['0']     = false,
  [0]       = false
}

local toboolean = function(value)
  return convertion[value]
end

return toboolean
