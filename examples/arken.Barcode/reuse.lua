-- arken.Barcode example: reusing one instance for multiple codes via setText

local Barcode = require('arken.Barcode')

local barcode = Barcode.new(300, 100, 'Code128')

local codes = { 'ARKEN-001', 'ARKEN-002', 'ARKEN-003' }

for _, code in ipairs(codes) do
  barcode:setText(code)
  local output = os.pwd() .. '/examples/arken.Barcode/reuse-' .. code .. '.png'
  barcode:save(output)
  print('arken.Barcode reuse OK -> ' .. output)
end
