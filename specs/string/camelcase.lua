local should = require "test.should"
local test   = {}

test['should upper case in underline'] = function()
  str1 = "this_is_a_test"
  str2 = "ThisIsATest"
  should.be_equal( str1:camelcase(), str2 )
end

test['should upper case in bar'] = function()
  str1 = "pedido/corporativo"
  str2 = "PedidoCorporativo"
  should.be_equal( str1:camelcase(), str2 )
end

test['should upper case in underline and bar'] = function()
  str1 = "pedido_regra/item"
  str2 = "PedidoRegraItem"
  should.be_equal( str1:camelcase(), str2 )
end

test['should upper first case'] = function()
  str1 = "classes/OByteArray"
  str2 = "ClassesOByteArray"
  should.be_equal( str1:camelcase(), str2 )
end

return test
