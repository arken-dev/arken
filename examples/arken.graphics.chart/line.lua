-- arken.graphics.chart.Line example
--
-- Ported from Gruff::Line usage:
--   g = Gruff::Line.new
--   g.title = "A Line Graph"
--   g.data 'Fries', [20, 23, 19, 8]
--   g.data 'Hamburgers', [50, 19, 99, 29]
--   g.write("test/output/line.png")

local Line = require('arken.graphics.chart.Line')

local output = os.pwd() .. '/examples/arken.graphics.chart/line.png'

local line = Line.new(800, 600)

line:setTitle('A Line Graph')
line:themeKeynote()

line:setHideDots(false)
line:setBaselineValue(50)
line:setBaselineColor('red')

line:setLabels({ [0] = '2024', [1] = '2025', [2] = '2026', [3] = '2027' })

line:data('Fries',      { 20, 23, 19, 8 })
line:data('Hamburgers', { 50, 19, 99, 29 })

line:write(output)

print('arken.graphics.chart.Line OK -> ' .. output)
