local test = {}

test.should_extension_file_name = function()
  local str = "teste.txt"
  local res = str:suffix(".")
  assert( res == 'txt', res )
end

test.should_extension_after_char = function()
  local str = "this is # a test"
  assert(str:suffix('#') == ' a test')
end

test['should return nil of not identify char'] = function()
  local str = "this is # a test"
  local res = str:suffix('*')
  assert(res == '', string.format("%s => type(%s)", res, type(res)))
end

test['should return complete suffix of many chars'] = function()
  local str = "this is a test"
  local res = str:suffix("is a")
  assert(res == " test", res)
end

test['should return last pattern'] = function()
  local str = "Web.Integracao.Meli"
  local res = str:suffix('.')
  assert( res == 'Meli', res )
end

test['should return query string'] = function()
  local url = '/importar/pedido_concluir?data=%7B%22tipo_pedido%22%3A%22venda%22%2C%22numero%22%3A%2216%22%7D'
  local res = url:suffix('?')
  assert( res == "data=%7B%22tipo_pedido%22%3A%22venda%22%2C%22numero%22%3A%2216%22%7D", res )
end

return test
