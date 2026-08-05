-- arken.graphics.chart.Pie example
--
-- Ported from Gruff::Pie usage:
--   g = Gruff::Pie.new
--   g.title = "Visual Pie Graph Test"
--   g.data 'Fries', 20
--   g.data 'Hamburgers', 50
--   g.write("test/output/pie_keynote.png")

local Pie = require('arken.graphics.chart.Pie')

local output = os.pwd() .. '/examples/arken.graphics.chart/pie.png'

local pie = Pie.new(800, 600)

pie:setTitle('Visual Pie Graph Test')
pie:themeKeynote()

pie:data('Fries',      20)
pie:data('Hamburgers', 50)
pie:data('Onion Rings', 30)

pie:write(output)

print('arken.graphics.chart.Pie OK -> ' .. output)
