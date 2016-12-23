-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local M = {}

M.color = function(value)
  if value == nil then
    return "#ffd4d4"
  end
  if value >= 1 then
    return "#d4dbff"
  end
  if value == -1 then
    return "#ffffff"
  end

  return "yellow"
end

M.color_coverage = function(value)
  if value < 100 then
    return "#ffd4d4"
  else
    return "#d4dbff"
  end
end

M.calc_coverage = function(dump)
  local total = #dump
  local uncoverage = 0
  for i, lines in ipairs(dump) do
    uncoverage = uncoverage + 1
  end
end

M.sort = function(tbl)
  table.sort(tbl, sort)
  return tbl
end

M.spacing = function(str)
  return tostring(str):gsub(" ", "&nbsp;")
end

M.number_format = function(value)
  return string.format("%.4f", value)
end

M.datetime = function()
  return os.date("%Y/%m/%d %H:%M:%S")
end

M.parameterize = function(value)
  return tostring(value):replace("/", "-")
end

M.sanitize = function(fileName)
  if fileName:startsWith('@') then
    fileName = fileName:mid(1, -1)
  end
  if fileName:startsWith('./') then
    fileName = fileName:mid(2, -1)
  end
  return fileName
end

return M
