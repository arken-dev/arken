local Routines = Class.new("routines.Routines")
Routines.help    = {}
Routines.output  = print

-------------------------------------------------------------------------------
-- LIST
-------------------------------------------------------------------------------

Routines.help.list = [[
  list all rotines
]]

function Routines:list()
  print('list...')
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
