local aes  = require('arken.crypto.aes')
local test = {}

test['should encrypt and decrypt returns original string'] = function()
  local text     = "arken platform"
  local password = os.uuid()

  local enc = aes.encrypt(text, password)
  local dec = aes.decrypt(enc, password)

  assert( enc ~= text )
  assert( dec == text, dec )
end

test['should encrypt and decrypt with 128 bits'] = function()
  local text     = "arken platform"
  local password = os.uuid()

  local enc = aes.encrypt(text, password, 128)
  local dec = aes.decrypt(enc, password, 128)

  assert( dec == text, dec )
end

test['should encrypt and decrypt with 192 bits'] = function()
  local text     = "arken platform"
  local password = os.uuid()

  local enc = aes.encrypt(text, password, 192)
  local dec = aes.decrypt(enc, password, 192)

  assert( dec == text, dec )
end

test['should encrypt and decrypt with 256 bits'] = function()
  local text     = "arken platform"
  local password = os.uuid()

  local enc = aes.encrypt(text, password, 256)
  local dec = aes.decrypt(enc, password, 256)

  assert( dec == text, dec )
end

test['should return a different ciphertext on each call'] = function()
  local text     = "arken platform"
  local password = os.uuid()

  local enc1 = aes.encrypt(text, password)
  local enc2 = aes.encrypt(text, password)

  assert( enc1 ~= enc2 )
end

test['should encrypt and decrypt binary data'] = function()
  local binary   = os.read('util/images/black.png')
  local password = os.uuid()

  local enc = aes.encrypt(binary, password)
  local dec = aes.decrypt(enc, password)

  assert( dec == binary )
end

test['should raise error for invalid bits'] = function()
  local text     = "arken platform"
  local password = os.uuid()

  local result, message = pcall(aes.encrypt, text, password, 265)
  assert( result == false )
  assert( message:contains("invalid bits") == true, message )
end

return test
