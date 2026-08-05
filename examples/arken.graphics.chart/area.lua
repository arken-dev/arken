-- arken.graphics.chart.Area example
--
-- Ported from Gruff::Area usage:
--   g = Gruff::Area.new
--   g.title = "CPU and Memory Usage"
--   g.data 'CPU',    [20, 35, 42, 30, 55, 48, 60]
--   g.data 'Memory', [40, 45, 41, 52, 58, 61, 65]
--   g.write("test/output/area.png")

local Area = require('arken.graphics.chart.Area')

local output = os.pwd() .. '/examples/arken.graphics.chart/area.png'

local area = Area.new(800, 600)

area:setTitle('CPU and Memory Usage')
area:themeKeynote()

area:setMinimumValue(0)
area:setMaximumValue(100)

area:setLabels({
  [0] = '00h', [1] = '04h', [2] = '08h', [3] = '12h',
  [4] = '16h', [5] = '20h', [6] = '24h',
})

area:data('CPU',    { 20, 35, 100, 30, 55, 48, 60 })
area:data('Memory', { 40, 45, 97, 52, 58, 61, 65 })

area:write(output)

print('arken.graphics.chart.Area OK -> ' .. output)
