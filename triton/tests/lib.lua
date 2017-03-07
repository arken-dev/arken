CHARON_ENV = os.getenv("CHARON_ENV") or "test"
local test  = require 'charon.test'
local start = os.microtime()

-------------------------------------------------------------------------------
-- TRITON START
-------------------------------------------------------------------------------
function triton_start()
  local list = os.glob("tests/lib", ".*.lua$", true)
  for i = 1, list:size() do
    print(list:at(i))
    triton.enqueue(list:at(i))
  end
end

-------------------------------------------------------------------------------
-- TRITON RUN
-------------------------------------------------------------------------------

function triton_run(file)
  local results = test.execute({file})

  local titulo = ""

  for file_name, result in pairs(results) do
    if titulo:len() > 0 then
      titulo = titulo, ', '
    end
  end

  for file_name, result in pairs(results) do
    for description, result in pairs(result) do

      triton.count('test')

      if result.status ~= 'ok' then
        local buffer = description .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          buffer = buffer .. tostring(result.msg) .. '\n'
        end
        triton.append('message', file .. '\n' .. buffer)
      end

      triton.count(result.status)
    end
  end
end

-------------------------------------------------------------------------------
-- TRITON STOP
-------------------------------------------------------------------------------

function triton_stop()
  local result = "%i tests, %i pendings, %i failures"
  print('\n' .. triton.result('message'))
  print(string.format(result, triton.total('test'), triton.total('failure'), triton.total('pending')))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end
