require "QDir"
process_http = function()
  return 200,
    {'Content-Type: text/html; charset=utf-8'},
    request['Query-String'] .. '<br>' .. 'User-Agent:' .. request['User-Agent']
end
