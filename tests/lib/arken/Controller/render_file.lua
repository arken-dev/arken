local Controller = require('arken.Controller')

local test = {}

test["deve retornar file xls"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.xls", APP_PATH) })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/vnd.ms-excel', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=exemplo.xls', headers[2] )
end

test["deve retornar file xml"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.xml", APP_PATH) })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/xml', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=exemplo.xml', headers[2] )

  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.xml", APP_PATH), name = "nome-do-arquivo-personalizado" })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/xml', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=nome-do-arquivo-personalizado.xml', headers[2] )
end

test["deve retornar file pdf"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.pdf", APP_PATH) })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/pdf', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=exemplo.pdf', headers[2] )
end

test["deve retornar file png"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.png", APP_PATH) })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: image/png', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=exemplo.png', headers[2] )
end

test["deve retornar file jpg"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.jpg", APP_PATH) })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: image/jpeg', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=exemplo.jpg', headers[2] )
end

test["deve retornar file json"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.json", APP_PATH) })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/json', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=exemplo.json', headers[2] )
end

test["deve retornar file ods"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', path = string.format("%s/util/tests/files/xls/exemplo.ods", APP_PATH) })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/vnd.oasis.opendocument.spreadsheet', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=exemplo.ods', headers[2] )
end

test["deve retornar file com view - xls"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', view = "web/integracao/vtex/modelo.xls" })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/vnd.ms-excel', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=modelo.xls', headers[2] )
end

test["deve retornar file com view - pdf"] = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render({ output = 'file', view = "web/integracao/vtex/modelo.pdf" })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: application/pdf', headers[1] )
  assert( headers[2] == 'Content-Disposition: attachment;filename=modelo.pdf', headers[2] )
end

return test
