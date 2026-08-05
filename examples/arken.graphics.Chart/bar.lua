-- arken.graphics.ChartBar example
--
-- Ported from Gruff::Bar usage:
--   g = Gruff::Bar.new
--   g.title = "Visual Multi-Line Graph Test"
--   g.data 'Fries', [20, 23, 19, 8]
--   g.data 'Hamburgers', [50, 19, 99, 29]
--   g.write("test/output/bar.png")

local ChartBar = require('arken.graphics.ChartBar')

local output = os.pwd() .. '/examples/arken.graphics.Chart/bar.png'

local bar = ChartBar.new(800, 600)

bar:setTitle('Visual Bar Graph Test')
bar:themeKeynote()
bar:setBarSpacing(0.8)

bar:setLabels({ [0] = '2024', [1] = '2025', [2] = '2026', [3] = '2027' })

bar:data('Fries',      { 20, 23, 19, 8 })
bar:data('Hamburgers', { 50, 19, 99, 29 })

bar:write(output)

print('arken.graphics.ChartBar OK -> ' .. output)
