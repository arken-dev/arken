local multipart = {}

multipart.parse = function(data)
  local list   = data:split('\r\n')
  local result = {}
  for i = 1, list:size() do
    if list:at(i):startsWith('Content-Disposition: form-data;') then
      local cols = list:at(i):split('"')
      local name = cols:at(2)
      if cols:at(4) then
        local file = {}
        file.name = cols:at(4)
        file.contentType = list:at(i+1)
        file.data = list:at(i+2)
        result[name] = file
      else
        result[name] = list:at(i+1)
      end
    end
  end
  return result
end

return multipart
