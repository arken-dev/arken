require "CHttpParser"

local multipart = require('charon.net.multi-part')
local header = os.read(CHARON_PATH .. '/tests/charon/classes/CHttpParser/example-2.header')
local parser = CHttpParser.new(header)
local test = {}

test.should_parse_requestUri = function()
  assert(parser:requestUri() == '/app/fornecedor/filtrar/indexPerform', parser:requestUri())
end

test.should_parse_requestUri = function()
  assert(parser:requestPath() == '/app/fornecedor/filtrar/indexPerform', parser:requestPath())
end

test.should_parse_field_id = function()
  local result = multipart.parse(parser:headerDone())
  assert(result.id == '12.341.234', result.id)
end

test.should_parse_first_field_data_cadastro_inicio = function()
  local result = multipart.parse(parser:headerDone())
  assert(result.data_cadastro_inicio == '01/12/2016', result.data_cadastro_inicio)
end

test.should_parse_first_field_data_cadastro_fim = function()
  local result = multipart.parse(parser:headerDone())
  assert(result.data_cadastro_fim == '31/12/2016', result.data_cadastro_fim)
end

test.should_parse_first_field_fornecedor = function()
  local result = multipart.parse(parser:headerDone())
  assert(result.fornecedor == 'Info Import', result.fornecedor)
end

test.should_parse_first_field_email = function()
  local result = multipart.parse(parser:headerDone())
  assert(result.email == 'infoimport@gmail.com', result.email)
end

test.should_parse_first_field_cpf_cnpj = function()
  local result = multipart.parse(parser:headerDone())
  assert(result.cpf_cnpj == '12345678/0001-35', result.cpf_cnpj)
end

test.should_parse_first_field_myfile = function()
  local result = multipart.parse(parser:headerDone())
  local myfile = result.myfile
  assert(type(myfile) == 'table', type(myfile))
  assert(myfile.name == 'teste.png', myfile.name)
  assert(myfile.contentType == 'Content-Type: image/png', myfile.contentType)
  assert(myfile.data:left(1000):contains('PNG'), myfile.data:left(10))
  assert(#myfile.data == 311, #myfile.data)
end

return test
