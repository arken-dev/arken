-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

function isblank(value)
  if type(value) == 'nil' then
    return true
  end

  if type(value) == 'string' then
     return value == ''
  end

  if type(value) == 'table' then
    return next(value) == nil
  end

  if type(value) == 'boolean' then
    return value == false
  end

  return false
end
