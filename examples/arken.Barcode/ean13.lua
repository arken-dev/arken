-- arken.Barcode example: EAN-13

local Barcode = require('arken.Barcode')

local output = os.pwd() .. '/examples/arken.Barcode/ean13.png'

local barcode = Barcode.new(200, 80, 'EAN13', '5901234123457')
barcode:save(output)

print('arken.Barcode EAN13 OK -> ' .. output)
