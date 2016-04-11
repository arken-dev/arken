require "QDir"
flag = ""
process_http = function()
  if request['Query-String'] == 'reload' then
    server.reload()
  end
  if request['Query-String'] == 'clear' then
    server.clear()
  end
  if request['Query-String'] == 'insert' then
    flag = "insert"
    cache.insert("teste", "Este é um teste do fim do mundo")
  end

  return 200,
    {'Content-Type: text/html; charset=utf-8'},
    '<br>' .. 'User-Agent:' .. request['User-Agent'] .. 'server: ' .. tostring(server) .. 'version: ' .. server.version() .. "cache: " .. tostring(cache.value("teste")) .. flag .. 'teste!!!'
end
