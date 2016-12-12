-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

function toboolean(value)
  if value == 'true' or value == 't' or value == true or value == 1 then
    return true
  end

  if value == 'false' or value == 'f' or value == false then
    return false
  end

  return nil
end
