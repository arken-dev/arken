-------------------------------------------------------------------------------
-- TEST MODULE
-------------------------------------------------------------------------------

local colorize = require 'colorize'
local test = {}

function test.process(file_name)
  local results   = {}
  local status, specs = pcall(dofile, file_name)
  -- arquivo com erro de sintaxe
  if not status then
    results[1] = {['file not load'] = specs, status = 'err', msg = specs}
    return results
  end
  local before    = specs.before    or function() end
  local after     = specs.after     or function() end
  local beforeAll = specs.beforeAll or function() end
  local afterAll  = specs.beforeAll or function() end
  specs.before    = nil
  specs.after     = nil
  specs.beforeAll = nil
  specs.afterAll  = nil
  status, message = pcall(beforeAll)
  for description, func in pairs(specs) do
    local status, message
    if type(func) == 'function' then
      status, message = pcall(before)
      if status then
        status, message = pcall(func)
      end
      -- require( 'oberon.record' ).cache = {}
      if status == false then
        if type(message) == 'table' then
          if message.kind == 'test' then
            results[description] = {status = 'fail', msg = message.msg}
            io.write(colorize.format('.', 'red'))
          else
            results[description] = {status = 'err', msg = message.msg}
            io.write(colorize.format('.', 'red'))
          end
        else
          results[description] = {status = 'err', msg = message}
          io.write(colorize.format('.', 'red'))
        end
      else
        results[description] = {status = "ok", msg = ''}
        io.write(colorize.format('.', 'green'))
      end
      status, message = pcall(after)
      if status == false then
        results['after'] = {status = 'err', msg = message}
        io.write(colorize.format('.', 'red'))
      end
    else
      results[description] = {status = 'pending', msg = func}
      io.write(colorize.format('.', 'yellow'))
    end
    io.flush()
  end
  status, message = pcall(afterAll)

  return results
end

function test.execute(argfiles)
  local files = {}
  for _, value in ipairs(argfiles) do
    local fileInfo = QFileInfo.new(value)
    if fileInfo:suffix() == "lua" then
      if fileInfo:exists() then
        table.insert(files, value)
      else
        error(value .. " not exists")
      end
    end
  end
  local result = {}
  for i, file in ipairs(files) do
    result[file] = test.process(file)
  end
  return result
end

return test
