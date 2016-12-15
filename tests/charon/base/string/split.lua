local test = {}

test.should_return_empty_string_if_is_empty_string = function()
  local str  = ""
  local list = str:split('-')
  assert( list:at(1) == nil, list:at(1) )
end

test.should_return_3_results = function()
  local str = 'a-b-c'
  local list = str:split('-')
  assert( list:at(1) == 'a', 'a => ' .. list:at(1) )
  assert( list:at(2) == 'b', 'b => ' .. list:at(2) )
  assert( list:at(3) == 'c', 'c => ' .. list:at(3) )
end

test.should_6_results_in_portugues_worlds_using_r_char = function()
  local str  = 'o rato roeu a roupa do rei de roma'
  local list = str:split('r')
  assert( list:at(1) == 'o ', list:at(1) )
  assert( list:at(2) == 'ato ', list:at(2) )
  assert( list:at(3) == 'oeu a ', list:at(3) )
  assert( list:at(4) == 'oupa do ', list:at(4) )
  assert( list:at(5) == 'ei de ', list:at(5) )
  assert( list:at(6) == 'oma', list:at(6) )
end

test.should_6_results_in_portugues_worlds_using_space = function()
  local str  = 'o rato roeu a roupa do rei de roma'
  local list = str:split(' ')
  assert( list:at(1) == 'o', list:at(1) )
  assert( list:at(2) == 'rato', list:at(2) )
  assert( list:at(3) == 'roeu', list:at(3) )
  assert( list:at(4) == 'a', list:at(4) )
  assert( list:at(5) == 'roupa', list:at(5) )
  assert( list:at(6) == 'do', list:at(6) )
  assert( list:at(7) == 'rei', list:at(7) )
  assert( list:at(8) == 'de', list:at(8) )
  assert( list:at(9) == 'roma', list:at(9) )
end

test.should_return_frase_if_no_find_char = function()
  local str  = 'o rato roeu a roupa do rei de roma'
  local list = str:split('#')
  assert( list:at(1) == 'o rato roeu a roupa do rei de roma', list:at(1) )
end

test.should_return_2_result = function()
  local str  = 'o rato roeu a roupa do rei de roma'
  local list = str:split('roupa')
  assert( list:at(1) == 'o rato roeu a ', list:at(1) )
  assert( list:at(2) == ' do rei de roma', list:at(2) )
end

test.should_return_empty_if_words_is_only_pattern = function()
  local str  = 'rrrrr'
  local list = str:split('r')
  assert( list:at(1) == nil )
  assert( list:at(2) == nil )
end

test.should_return_first_and_last_words_with_center_is_only_patter = function()
  local str  = 'abc-------def'
  local list = str:split('-')
  assert( list:at(1) == 'abc' )
  assert( list:at(2) == 'def' )
  assert( list:at(3) == nil )
end

test.should_return_first_and_last_words_with_begin_and_center_and_last_is_only_patter = function()
  local str  = '---abc-------def---'
  local list = str:split('-')
  assert( list:at(1) == 'abc' )
  assert( list:at(2) == 'def' )
  assert( list:at(3) == nil )
end

return test
