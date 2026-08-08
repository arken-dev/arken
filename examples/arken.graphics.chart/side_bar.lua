-- arken.graphics.chart.SideBar example
--
-- Ported from Gruff::SideBar usage:
--   g = Gruff::SideBar.new
--   g.title = "Side Bar Graph Test"
--   g.data 'Fries', [20, 23, 19, 8]
--   g.data 'Hamburgers', [50, 19, 99, 29]
--   g.write("test/output/side_bar.png")

local SideBar = require('arken.graphics.chart.SideBar')

local output = os.pwd() .. '/examples/arken.graphics.chart/side_bar.png'

local bar = SideBar.new(800, 600)

bar:setTitle('Side Bar Graph Test')
bar:themeKeynote()
bar:setBarSpacing(0.8)

bar:setLabels({ [0] = '2024', [1] = '2025', [2] = '2026', [3] = '2027' })

bar:data('Fries',      { 20, 23, 19, 8 })
bar:data('Hamburgers', { 50, 19, 99, 29 })

bar:write(output)

print('arken.graphics.chart.SideBar OK -> ' .. output)
