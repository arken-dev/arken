-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local empty = function(value)
  if type(value) == 'table' then
    return next(value) == nil
  else
    return ( value == '' or value == nil )
  end
end

return empty
