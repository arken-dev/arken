local aes  = require('arken.crypto.aes')
local test = {}

test['should decrypt data encrypted with the same password'] = function()
  local text     = "arken platform"
  local password = os.uuid()

  local enc = aes.encrypt(text, password)
  local dec = aes.decrypt(enc, password)

  assert( dec == text, dec )
end

test['should decrypt binary data'] = function()
  local binary   = os.read('util/images/black.png')
  local password = os.uuid()

  local enc = aes.encrypt(binary, password)
  local dec = aes.decrypt(enc, password)

  assert( dec == binary )
end

test['should raise error when password is wrong'] = function()
  local text     = "arken platform"
  local password = os.uuid()
  local enc      = aes.encrypt(text, password)

  local result, message = pcall(aes.decrypt, enc, os.uuid())
  assert( result == false )
  assert( message:contains("wrong password or corrupted data") == true, message )
end

test['should raise error for corrupted data'] = function()
  local result, message = pcall(aes.decrypt, "x", os.uuid())
  assert( result == false )
  assert( message:contains("wrong password or corrupted data") == true, message )
end

test['should raise error for invalid bits'] = function()
  local password = os.uuid()
  local enc       = aes.encrypt("arken platform", password)

  local result, message = pcall(aes.decrypt, enc, password, 265)
  assert( result == false )
  assert( message:contains("invalid bits") == true, message )
end

return test
