local Routines = Class.new("routines.Routines")
Routines.help    = {}
Routines.output  = print

local helpOutput = function(module)
  local help = ""
  local size = 0

  if module.help then

    for k, v in pairs(module.help) do
      if #k > size then
        size = #k
      end
    end

    local margem = '\n' .. string.rep(' ', size + 1)

    for k, v in pairs(module.help) do
      local space = string.rep(' ', size - #k)
      help = help .. k .. space .. ' # ' .. v:trimmed():replace('\n', margem) .. '\n'
    end

    Routines.output(help)
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
  print('list...')
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
      Routines.output(module.className:underscore())
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
  print('search')
end

return Routines
