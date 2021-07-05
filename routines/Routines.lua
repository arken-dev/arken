local Routines = Class.new("routines.Routines")
Routines.help    = {}
Routines.output  = print

local helpOutput = function(module, search)
  local help = ""
  local size = 0

  if module.help then

    for k, v in pairs(module.help) do
      if #k > size then
        size = #k
      end
    end

    local margem = '\n' .. string.rep(' ', size + 1)
    local flag = false

    for k, v in pairs(module.help) do
      if search == nil or (k:contains(search) or v:contains(search)) then
        flag = true
        local space = string.rep(' ', size - #k)
        help = help .. k .. space .. ' # ' .. v:trim():replace('\n', margem) .. '\n'
      end
    end

    if flag then
      Routines.output(module.className:underscore():replace('routines_', ''))
      Routines.output(help)
    end
  end
end


-------------------------------------------------------------------------------
-- LIST
-------------------------------------------------------------------------------

Routines.help.list = [[
  list all rotines
]]

function Routines:list()
  local path = {}
  local list = tostring(package.path):split(';')
  for i = 1, list:size() do
    local str = list:at(i)
    str = str:replace("/?.lua", ""):replace('.', '/')
    str = os.abspath(str)
    str = str .. '/routines'
    if os.exists(str) then
      table.insert(path, str)
    end
  end
  for _, str in ipairs(path) do
    local list = os.glob(str, "\\.lua$", false)
    for i = 1, list:size() do
      local module = dofile(list:at(i))
      helpOutput(module)
    end
  end
end

-------------------------------------------------------------------------------
-- SEARCH
-------------------------------------------------------------------------------

Routines.help.search = [[
  search rotines
]]

function Routines:search()
  local path = {}
  local search = self:params()[1]
  local list = tostring(package.path):split(';')
  for i = 1, list:size() do
    local str = list:at(i)
    str = str:replace("/?.lua", ""):replace('.', '/')
    str = os.abspath(str)
    str = str .. '/routines'
    if os.exists(str) then
      table.insert(path, str)
    end
  end
  for _, str in ipairs(path) do
    local list = os.glob(str, "\\.lua$", false)
    for i = 1, list:size() do
      local module = dofile(list:at(i))
      helpOutput(module, search)
    end
  end
end

return Routines
