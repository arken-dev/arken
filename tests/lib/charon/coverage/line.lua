local coverage = require('charon.coverage')
local test = {}

test.should_return_one_for_line_function = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("M.test = function()", nil, keywords)  == 1 )
end

test.should_return_nil_for_line_function_description = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("my_descrition_for_function_local", nil, keywords) == nil)
end

test.should_return_one_for_var_equal_function = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("teste = function()", nil, keywords) == 1)
end

test.should_return_one_for_var_equal_function_without_space = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("teste=function()", nil, keywords) == 1)
end

test.should_return_one_negative_if_str1_is_true = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = true, str2 = false }
  coverage.flag = true
  assert( coverage.line("local teste = function() ", nil, keywords) == -1 )
  assert( coverage.line("local teste = 1 ", nil, keywords) == -1)
end

test.should_return_one_negative_if_str2_is_true = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = true }
  coverage.flag = true
  assert( coverage.line("local teste = function() ", nil, keywords) == -1 )
  assert( coverage.line("local teste = 1 ", nil, keywords) == -1)
end

test.should_return_one_negative_if_is_end = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("end", nil, keywords) == -1)
end

test.should_return_one_negative_if_is_end_with_space = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("end ", nil, keywords) == -1)
end

test.should_return_one_negative_if_is_else = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("else", nil, keywords) == -1)
end

test.should_return_one_negative_if_is_else_with_space = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("else ", nil) == -1)
end

test.should_return_one_negative_if_start_comment = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("--", nil, keywords) == -1)
end

test.should_return_one_negative_if_is_a_empty = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("", nil, keywords) == -1)
end

test.should_return_one_negative_if_is_a_blank = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("  ", nil, keywords) == -1)
end

test.should_open_flag_if_parentheses_and_close_after = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("(", nil, keywords) == 1)
  assert( coverage.line("{", nil, keywords) == -1)
  assert( coverage.line("}", nil, keywords) == -1)
  assert( coverage.line(")", nil, keywords) == -1)
  assert( coverage.line("var = 'teste'", 1, keywords) == 1)
end

test.should_open_flag_if_parentheses_if_open_with_space = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("( ", nil, keywords) == 1)
end

test.should_return_one_negative_if_start_commet_with_space = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("  --", nil, keywords) == -1)
end

test.should_return_one_if_string_inside_string = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line([["'"]], 1, keywords) == 1)
  assert( coverage.line([['"']], 1, keywords) == 1)
end

test.should_return_one_if_string_is_a_brackets = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("[[abc]]", 1, keywords) == 1)
end

test.should_equal_keywords_flags_if_flags_one_two_and_tree_inside_string = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line([["{(["]], 1, keywords) == 1)
  assert( keywords.flag1 == 0 )
  assert( keywords.flag2 == 0 )
  assert( keywords.flag3 == 0 )
  assert( keywords.str1  == false )
  assert( keywords.str2  == false )
end

test.should_equal_one_if_javascript = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line([[local var = "jQuery('#div1') {"]], 1, keywords) == 1)
  assert( coverage.line([["var str = '1';"]], 1, keywords) == 1)
  assert( coverage.line([["}"]], 1, keywords) == 1)
end

test.should_equal_one_if_javascript_in_multiline_string = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("local var = [[jQuery('#div1') { var val = 1;", 1, keywords) == 1)
  assert( coverage.line("{", nil, keywords) == -1)
  assert( coverage.line("var str = '1';", nil, keywords) == -1)
  assert( coverage.line("}]]", nil, keywords) == -1)
end

test.should_keyword_is_open_if_string_brackets_is_open = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("local var = [[jQuery('#div1')", 1, keywords) == 1)
  assert( keywords.flag1 == 0 )
  assert( keywords.flag2 == 0 )
  assert( keywords.flag3 == 2 )
  assert( keywords.str1  == false )
  assert( keywords.str2  == false )
  assert( coverage.line("var = 1;", nil, keywords) == -1)
  assert( keywords.flag1 == 0 )
  assert( keywords.flag2 == 0 )
  assert( keywords.flag3 == 2 )
  assert( keywords.str1  == false )
  assert( keywords.str2  == false )
  assert( coverage.line("str = 'abc';]]", nil, keywords) == -1)
  assert( keywords.flag1 == 0 )
  assert( keywords.flag2 == 0 )
  assert( keywords.flag3 == 0 )
  assert( keywords.str1  == false )
  assert( keywords.str2  == false )
end

test.should_return_one_negative_if_start_comment_and_function_word_inside = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line("-- This function will also remove multiple slashes", nil, keywords) == -1)
end

test.should_return_uncoverage_line_if_function_is_a_parameter = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line('path = path:gsub("([^/]*%./)", function (s)', nil, keywords) == nil)
end

test.should_return_one_negative_if_open_brackets = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line('--[[', nil, keywords) == -1)
  assert( keywords.flag == true )
  assert( coverage.line('local info = debug.getinfo(2, "Sl")', nil, keywords) == -1)
  assert( coverage.line('local filename = info.source', nil, keywords) == -1)
  assert( coverage.line('local linedefined = info.currentline', nil, keywords) == -1)
  assert( coverage.line(']]', nil, keywords) == -1)
  assert( coverage.line('local var = "abc"', nil, keywords) == nil)
end

test.should_return_one_negative_if_function_inside_brackets_comments = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line('--[[', nil, keywords) == -1)
  assert( coverage.line('function Object:isBlank(column)', nil, keywords) == -1)
  assert( coverage.line(']]', nil, keywords) == -1)
end

test.should_return_one_negative_if_function_inside_brackets_comments = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line('mytest:mymethod(function()', nil, keywords) == nil )
  assert( coverage.line('end)', nil, keywords) == nil )
end

test.should_return_origin_flag_if_function_is_declared = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line('function Object:execute(method, params)', 1, keywords) == 1 )
end

test.should_return_origin_flag_if_function_is_declared_after_comment = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line('-- comment', nil, keywords) == -1 )
  assert( coverage.line('--[[', nil, keywords) == -1 )
  assert( coverage.line('local var = "teste"', nil, keywords) == -1 )
  assert( coverage.line(']]', nil, keywords) == -1 )
  assert( coverage.line('coverage.start = function(fake)', nil, keywords) == 1 )
end

test.should_return_origin_flag_if_function_is_declared_after_comment = function()
  local keywords = { flag1 = 0, flag2 = 0, flag3 = 0, str1 = false, str2 = false }
  assert( coverage.line('-- comment', nil, keywords) == -1 )
  assert( coverage.line('--[[', nil, keywords) == -1 )
  assert( coverage.line('local var = "teste"', nil, keywords) == -1 )
  assert( coverage.line(']]', nil, keywords) == -1 )
  assert( coverage.line('function Object:initialize()', nil, keywords) == 1 )
end

return test
