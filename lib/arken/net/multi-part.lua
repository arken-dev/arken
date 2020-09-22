local ByteArray = require 'arken.ByteArray'
local multipart = {}

multipart.parse = function(data)
  local indexOf  = data:indexOf('\r\n')
  local boundary = data:left(indexOf-1)
  local list     = data:split(boundary)
  local result = {}
  for i = 1, list:size() do
    local frag  = list:at(i)
    local rows  = frag:split('\r\n')
    local disp  = rows:at(1)
    local cols  = disp:split('"')
    local field = cols:at(2)
    if( cols:at(4) ) then
      local file = {}
      file.name = cols:at(4)
      file.contentType = rows:at(2)
      local tamanho = #disp + #file.contentType + 8
      file.data = ByteArray.new(frag):mid(tamanho, #frag - tamanho - 2)
      result[field] = file
    else
      if field then
        result[field] = rows:at(2)
      end
    end
  end
  return result
end

return multipart
