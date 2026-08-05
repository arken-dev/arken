-- arken.graphics.chart.StackedBar example
--
-- Ported from Gruff::StackedBar usage:
--   g = Gruff::StackedBar.new
--   g.title = "Stacked Bar Graph Test"
--   g.data 'Fries', [20, 23, 19, 8]
--   g.data 'Hamburgers', [50, 19, 99, 29]
--   g.write("test/output/stacked_bar.png")

local StackedBar = require('arken.graphics.chart.StackedBar')

local output = os.pwd() .. '/examples/arken.graphics.chart/stacked_bar.png'

local bar = StackedBar.new(800, 600)

bar:setTitle('Stacked Bar Graph Test')
bar:themeKeynote()
bar:setBarSpacing(0.8)

bar:setLabels({ [0] = '2024', [1] = '2025', [2] = '2026', [3] = '2027' })

bar:data('Fries',      { 20, 23, 19, 8 })
bar:data('Hamburgers', { 50, 19, 99, 29 })

bar:write(output)

print('arken.graphics.chart.StackedBar OK -> ' .. output)
