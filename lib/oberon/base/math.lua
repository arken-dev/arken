-------------------------------------------------------------------------------
-- Mathematical Functions.
-- This library is an interface to the standard C math library.
-- It provides all its functions inside the table math.
-- @module math

math = require 'math'

-------------------------------------------------------------------------------
-- Returns round number with precision.
-- @function [parent=#math] round
-- @param #number number
-- @param #number precision
-- @return #number

function math.round(number, precision)
  return math.floor((number + (0.5/(10 ^ precision))) * (10 ^ precision)) / (10 ^ precision)
end

-------------------------------------------------------------------------------
-- Returns round number truncate with precision.
-- @function [parent=#math] truncate
-- @param #number number
-- @param #number precision
-- @return #number

function math.truncate(number, precision)
  return math.floor(number * (10 ^ precision)) / (10 ^ precision)
end

return math
