local Class = require('arken.oop.Class')
local Array = Class.new("Array")

function Array:each()
  local i = 0
  local n = table.getn(self)
  return function()
    i = i + 1
    if i <= n then return self[i] end
  end
end

function Array:at(i)
  return self[i]
end

function Array:size()
  return #self
end

function Array:count()
  local count = 0
  for k, v in pairs(self) do
    count = count + 1
  end
  return count
end

function Array:keys()
  local keys = Array.new()
  for index, value in pairs(self) do
     keys:insert(index)
  end
  return keys
end

function Array:contains(value)
  for _, v in pairs(self) do
    if value == v then
      return true
    end
  end
  return false
end

function Array:uniq()
  local uniq = Array.new()
  for i, value in pairs(self) do
    if not uniq:contains(value) then
      uniq:insert(value)
    end
  end
  return uniq
end

function Array:join(str)
  local raw = ''
  for _, value in pairs(self) do
    if not empty(raw) then
      raw = raw .. str
    end
    raw = raw .. tostring(value)
  end
  return raw
end

function Array:insert(value)
  table.insert(self, value)
end

return Array
