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

local sort = function(a, b)
  return tostring(a.file_name) < tostring(b.file_name)
end

M.sort = function(tbl)
  table.sort(tbl, sort)
  return tbl
end

return M
