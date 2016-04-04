process_http = function()
  return 200, {'Content-Type: text/html; charset=utf-8'}, "uuid: " .. os.uuid()
end
