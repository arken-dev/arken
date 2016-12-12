require 'charon.pgsql'

connect = pg.connect('dbname=fusion_development user=postgres password=42 host=localhost')
query   = [[SELECT attname FROM pg_attribute]]
result  = connect:exec(query)
for row in result:rows() do
  print(row.attname)
end 
