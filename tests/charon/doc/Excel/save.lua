local Excel = require('charon.doc.Excel')
local test  = {}

test.should_write_cell_string = function()
  local filename  = os.uuid()
  local workbook  = Excel.workbook()
  local worksheet = workbook:addWorksheet("Planilha1")
  local cell      = worksheet:cell(1, 1)
  cell:setString("Hello World !")
  workbook:save(filename)

  local workbook1  = Excel.workbook()
  workbook1:load(filename)
  local worksheet1 = workbook:getWorksheet("Planilha1")
  local cell1      =  worksheet1:cell(1, 1)

  assert( cell1:getString() == "Hello World !", cell1:getString() )
end

return test
