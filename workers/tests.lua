ARKEN_ENV = os.getenv("ARKEN_ENV") or "test"

local test  = require 'arken.test'
local start = os.microtime()
local M = {}

-------------------------------------------------------------------------------
-- START
-------------------------------------------------------------------------------

function M.start(triton, params)
  print('tests in path ' .. params.path )
  local list = os.glob(params.path, true)
  for i = 1, list:size() do
    local filePath = list:at(i)
    if filePath:endsWith(".lua") then
      triton:enqueue(filePath)
    end
  end
end

-------------------------------------------------------------------------------
-- BEFORE
-------------------------------------------------------------------------------

function M.before(triton)
end

-------------------------------------------------------------------------------
-- RUN
-------------------------------------------------------------------------------

function M.run(triton, fileName)

  local results = test.execute({fileName})
  local titulo  = ""

  for file_name, result in pairs(results) do
    if titulo:len() > 0 then
      titulo = titulo, ', '
    end
    titulo = titulo .. file_name:replace('tests/models/', '')
  end

  for file_name, result in pairs(results) do
    for description, result in pairs(result) do

      triton:count('test')

      if result.status ~= 'ok' then
        local buffer = description .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          buffer = buffer .. tostring(result.msg) .. '\n'
        end
        triton:append('message', fileName .. '\n' .. buffer)
      end

      triton:count(result.status)
    end

  end
end

-------------------------------------------------------------------------------
-- AFTER
-------------------------------------------------------------------------------

function M.after(triton)
end

-------------------------------------------------------------------------------
-- STOP
-------------------------------------------------------------------------------

function M.stop(triton)
  local result = "%i tests, %i pendings, %i failures"
  print('\n' .. triton:result('message'))
  print(string.format(result, triton:total('test'), triton:total('failure'), triton:total('pending')))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end

return M
