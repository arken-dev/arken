-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

CHARON_ENV = os.getenv("CHARON_ENV") or "test"

--local ActiveRecord = require("charon.ActiveRecord")
--ActiveRecord.adapter().loader()

local test  = require 'charon.test'

local callisto = function(file)
  local init = os.microtime()
  package.reload()
  local results = test.execute({file})
  local buffer  = ""
  local icon    = "ok"

  count = {}
  count.ok      = 0
  count.failure = 0
  count.pending = 0

  for file_name, result in pairs(results) do
    for description, result in pairs(result) do
      count[result.status] = count[result.status] + 1
      if result.status ~= 'ok' then
        buffer = buffer .. '<b>' .. description .. '</b>' .. '\n'
        if result.msg and tostring(result.msg):len() > 0  then
          --buffer = buffer .. ' ' .. result.status .. '\n'
          buffer = buffer .. tostring(result.msg) .. '\n\n'--print(result.msg)
        end
      end
    end

    if count.pending > 0 or count.ok == 0 then
      icon = "warning"
    end

    if count.failure > 0 then
      icon = "failure"
    end

    test.output("\n")
    rodape = ""
    for i, v in pairs(count) do
      if rodape:len() > 1 then
        rodape = rodape .. ', '
      end
        rodape = rodape .. v .. " " .. i
      end
    end
    buffer = buffer .. rodape
    buffer = buffer .. '\n\nFinished in ' .. string.format("%.3f", os.microtime() - init) .. ' seconds\n'
    test.output(buffer)
    return icon, buffer:replace('\n', '<br>')
end

return callisto
