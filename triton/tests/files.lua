OBERON_ENV = os.getenv("OBERON_ENV") or "test"

local test  = require 'test'
local start = os.microtime()

-------------------------------------------------------------------------------
-- TRITON START
-------------------------------------------------------------------------------

function triton_start()
  for _, file in ipairs(arg) do
    print(file)
    triton.appendPath(file)
  end
end

-------------------------------------------------------------------------------
-- TRITON RUN
-------------------------------------------------------------------------------

function triton_run(file)

  local results = test.execute({file})

  local titulo = ""
  local icon   = "ok"

  count = {}
  count.ok      = 0
  count.err     = 0
  count.fail    = 0
  count.pending = 0

  for file_name, result in pairs(results) do
    if titulo:len() > 0 then
      titulo = titulo, ', '
    end
    titulo = titulo .. file_name:swap('specs/models/', '')
  end

  for file_name, result in pairs(results) do
    for description, result in pairs(result) do
      count[result.status] = count[result.status] + 1

      triton.addOk()

      if result.status ~= 'ok' then
        local buffer = description .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          buffer = buffer .. tostring(result.msg) .. '\n'
        end
        triton.appendResult(file .. '\n')
        triton.appendResult(buffer)
      end

      if result.status == 'err' then
        triton.addError()
      end

      if result.status == 'pending' then
        triton.addPending()
      end

    end
  end
end

-------------------------------------------------------------------------------
-- TRITON STOP
-------------------------------------------------------------------------------

function triton_stop()
  print('')
  print(string.format("Results %s", triton.result()))
  print(string.format("%i tests, %i failures, %i pendings", triton.ok(), triton.failure(), triton.pending() ))
  print(string.format("Finished in %.2f seconds", os.microtime() - start))
end
