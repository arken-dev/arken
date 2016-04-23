flag = true
var  = nil
if flag then
  var = 'true...'
else
  var = 'false..'
end

function calcular()
  return 1 + 1
end

for i = 1, 10 do
calcular()
end

calcular{
  id = 1,
  cancelado = false
}

calcular(
  1,
  2,
  3
)
