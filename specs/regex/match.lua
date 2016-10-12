require 'oberon.regex'

local should = require "test.should"
local test   = {}

test['should be true for (g|p|r)ato'] = function()
  should.be_equal(regex.match("rato", "(g|p|r)ato"), true)
  should.be_equal(regex.match("gato", "(g|p|r)ato"), true)
  should.be_equal(regex.match("pato", "(g|p|r)ato"), true)
  should.be_equal(regex.match("iato", "(g|p|r)ato"), false)
end

return test
