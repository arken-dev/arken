-- arken.Barcode example: QR Code (2D)

local Barcode = require('arken.Barcode')

local output = os.pwd() .. '/examples/arken.Barcode/qrcode.png'

local barcode = Barcode.new(200, 200, 'QRCode', 'https://objectdata.com.br')
barcode:save(output)

print('arken.Barcode QRCode OK -> ' .. output)
