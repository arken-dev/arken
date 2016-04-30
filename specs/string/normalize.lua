local should = require "test.should"
local test   = {}

test['should normalize form[descricao] example'] = function()
  local str1 = "form[descricao]"
  local str2 = "form_descricao"
  should.be_equal(str1:normalize(), str2)
end

test['should normalize form[product][stock]'] = function()
  local str1 = "form[product][stock]"
  local str2 = "form_product_stock"
  should.be_equal(str1:normalize(), str2)
end

test['should preserve upper case form[Product][Stock]'] = function()
  local str1 = "form[Product][Stock]"
  local str2 = "form_Product_Stock"
  should.be_equal(str1:normalize(), str2)
end

return test
