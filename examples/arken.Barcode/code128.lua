-- arken.Barcode example: Code 128 (1D)

local Barcode = require('arken.Barcode')

local output = os.pwd() .. '/examples/arken.Barcode/code128.png'

local barcode = Barcode.new(300, 100, 'Code128', 'ARKEN-128')
barcode:save(output)

print('arken.Barcode Code128 OK -> ' .. output)
