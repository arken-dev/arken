require "QDir"
process_http = function()
  if request['Query-String'] == 'reload' then
    server.reload()
  end
  if request['Query-String'] == 'clear' then
    server.clear()
  end

  return 200,
    {'Content-Type: text/html; charset=utf-8'},
    '<br>' .. 'User-Agent:' .. request['User-Agent'] .. 'server: ' .. tostring(server)
end
