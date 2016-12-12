-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

flag = true
var  = nil
if flag then
  var = 'true...'
else
  var = 'false..'
end

function calcular()
  return 1 + 1
end

for i = 1, 10 do
calcular()
end

calcular{
  id = 1,
  cancelado = false
}

calcular(
  1,
  2,
  3
)
