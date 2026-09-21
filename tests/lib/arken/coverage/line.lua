local coverage = require('arken.coverage')
local json     = require('arken.json')
local test     = {}

test.before = function()
  coverage.reset()
end

test.should_uncoverage_block_comment = function()
  local result = coverage.analyze("util/coverage/Adapter.lua")
  local lines  = result.lines

  assert(lines[145].src == "--[[")
  assert(lines[145].flag == 1 )
  assert(lines[145].default == -1, lines[145].comment)

  assert(lines[146].src:squish() == "values.join = nil", lines[146].src:squish())
  assert(lines[146].level == 1, lines[145].level )
  assert(lines[146].flag == -1 )
end

test.should_resolv_start_end_block_string_on_the_same_line = function()
  local result = coverage.analyze("util/coverage/PostgresAdapter.lua")
  local lines  = result.lines

  assert(lines[151].src:squish() == "WHERE a.attrelid = ']] .. self.tableName .. [['::regclass", lines[151].src)
  assert(lines[151].flag == -1, lines[151].flag )
  assert(lines[151].default == -1, lines[151].comment)
  assert(lines[151].comment == 'starts with --[[ or endsWith [[ or endsWith {', lines[151].comment)
  assert(lines[151].default == -1, lines[151].default)

  assert(lines[152].src:squish() == "AND a.attnum > 0 AND NOT a.attisdropped", lines[152].src:squish())
  assert(lines[152].default == -1, lines[152].comment )
  assert(lines[152].flag == -1, lines[152].comment )
  assert(lines[152].comment == "default is -1", lines[152].comment )

  assert(lines[153].src:squish() == "ORDER BY a.attnum", lines[153].src:squish())
  assert(lines[153].default == -1, lines[153].comment )
  assert(lines[153].flag == -1, lines[153].comment )
  assert(lines[153].comment == "default is -1", lines[153].comment )

  assert(lines[154].src:squish() == "]]", lines[154].src:squish())
  assert(lines[154].level == 2, lines[154].level )
  assert(lines[154].flag == 1, lines[154].flag )
  assert(lines[154].default == nil, lines[154].default )
  assert(lines[154].comment == "end with ]]", lines[154].comment )
end

test.should_return_line_is_blank = function()
  local result = coverage.analyze("util/coverage/PostgresAdapter.lua")
  local lines  = result.lines
  assert(lines[5].src == "", lines[5].src)
  assert(lines[5].flag == -1, lines[5].flag )
  assert(lines[5].comment == 'line is blank', lines[5].comment)
  assert(lines[5].default == nil, lines[5].default)
end

test.should_return_line_is_blank_2 = function()
  local result = coverage.line("", nil)
  assert(result == -1)
  assert(coverage.comment == 'line is blank', coverage.comment)
end

test.should_end_return = function()
  local result = coverage.analyze("util/coverage/PostgresAdapter.lua")
  local lines  = result.lines
  assert(lines[313].src == "return ActiveRecord_PostgresAdapter", lines[313].src)
  assert(lines[313].flag == 1, lines[313].flag )
  assert(lines[313].comment == 'default is nil and start with return', lines[313].comment)
  assert(lines[313].default == nil, lines[313].default)
end

test.should_default_if_open_and_close_string_block_and_level_one = function()
  local result = coverage.analyze("util/coverage/Migrate.lua")
  local lines  = result.lines
  local src    = lines[47].src:squish()
  assert(src == "local sql = [[INSERT INTO schema_migration VALUES ('%s')]]", lines[47].src)
  assert(lines[47].flag == nil, lines[47].flag )
  assert(lines[47].comment == 'end with ]]', lines[47].comment)
  assert(lines[47].default == nil, lines[47].default)
end

test.should_follow_the_line_that_opened_the_parenthesis = function()
  local result = coverage.analyze("util/coverage/PostgresAdapter.lua")
  local lines  = result.lines

  assert(lines[23].src:squish() == "ActiveRecord_PostgresAdapter.instanceConnection, errmsg = env:connect(", lines[23].src)
  assert(lines[23].flag == nil, lines[23].flag)

  assert(lines[24].src:squish() == "self.database, self.user, password, self.host", lines[24].src)
  assert(lines[24].flag == nil, lines[24].flag)
  assert(lines[24].comment == 'continues open parenthesis', lines[24].comment)

  assert(lines[25].src:squish() == ")", lines[25].src)
  assert(lines[25].flag == nil, lines[25].flag)
  assert(lines[25].comment == 'continues open parenthesis', lines[25].comment)
end

test.should_repeat_the_flag_of_the_line_that_opened_the_parenthesis = function()
  coverage.default = nil
  coverage.braces  = false
  coverage.level   = 1
  coverage.parens  = 0

  assert(coverage.line("local texto = string.format(", 4) == 4)
  assert(coverage.line("  'total de %d horas',", nil) == 4)
  assert(coverage.line("  horas", nil) == 4)
  assert(coverage.line(")", nil) == 4)
  assert(coverage.line("Model.info(texto)", nil) == nil)
end

test.should_not_repeat_the_flag_on_the_body_of_a_inline_function = function()
  coverage.default = nil
  coverage.braces  = false
  coverage.level   = 1
  coverage.parens  = 0

  assert(coverage.line("return list:join(',', function(item)", 1) == 1)
  assert(coverage.line("  return tostring(item.id)", nil) == nil)
end

return test
