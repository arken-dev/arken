-- arken.Barcode example: Data Matrix (2D, square)

local Barcode = require('arken.Barcode')

local output = os.pwd() .. '/examples/arken.Barcode/datamatrix.png'

local barcode = Barcode.new(150, 150, 'DataMatrix', 'ARKEN-DATAMATRIX')
barcode:save(output)

print('arken.Barcode DataMatrix OK -> ' .. output)
