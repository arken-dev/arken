-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local helper = {}

helper.color = function(value)
  if value == nil then
    return "#ffd4d4"
  end
  if value >= 1 then
    return "#d4dbff"
  end
  if value == -1 then
    return "#ffffff"
  end
end

helper.color_coverage = function(value)
  if value < 100 then
    return "#ffd4d4"
  else
    return "#d4dbff"
  end
end

helper.spacing = function(str)
  return tostring(str):gsub(" ", "&nbsp;")
end

helper.number_format = function(value)
  return string.format("%.4f", value)
end

helper.datetime = function()
  return os.date("%Y/%m/%d %H:%M:%S")
end

helper.parameterize = function(value)
  return tostring(value):replace("/", "-")
end

helper.sanitize = function(fileName)
  if fileName:startsWith('@') then
    fileName = fileName:mid(2, -1)
  end
  if fileName:startsWith('./') then
    fileName = fileName:mid(3, -1)
  end
  return fileName
end

return helper
