local test = {}

test['format default notation 1234.56 with around'] = function()
  local value = math.format(1234.56)
  assert( value == '1,235', value )
end

test['format default notation 1234.56 without around'] = function()
  local value = math.format(1234.26)
  assert( value == '1,234', value )
end

test['format 1234.56 with 2 params'] = function()
  local value = math.format(1234.56, 2)
  assert( value == '1,234.56', value )
end

test['format 1234.56 with 3 params'] = function()
  local value = math.format(1234.56, 2, '*')
  assert( value == '1,234*56', value )
end

test['format 1234.56 with 4 params'] = function()
  local value = math.format(1234.56, 2, '*', ' ')
  assert( value == '1 234*56', value )
end

test['format 2500.35 to 2.500,35'] = function()
  local value = math.format(2500.35, 2, ',', '.')
  assert( value == '2.500,35', value )
end

test['format 2500.00 to 2.500,00'] = function()
  local value = math.format(2500.00, 2, ',', '.')
  assert( value == '2.500,00', value )
end

test['format 2500.56 to 2.500,6'] = function()
  local value = math.format(2500.56, 1, ',', '.')
  assert( value == '2.500,6', value )
end

test['format 2500.56 to 2.500,560'] = function()
  local value = math.format(2500.56, 3, ',', '.')
  assert( value == '2.500,560', value )
end

test['format 2500.56789 to 2.500,56789'] = function()
  local value = math.format(2500.56789, 5, ',', '.')
  assert( value == '2.500,56789', value )
end

test['format 2500.123456789 to 2.500,123456789'] = function()
  local value = math.format(2500.123456789, 9, ',', '.')
  assert( value == '2.500,123456789', value )
end

test['format 32500.25 to 32.500,25'] = function()
  local value = math.format(32500.25, 2, ',', '.')
  assert( value == '32.500,25', value )
end

test['format 32500.25 to 32.500,250'] = function()
  local value = math.format(32500.25, 3, ',', '.')
  assert( value == '32.500,250', value )
end

test['format 32500.25 to 32.500,2500'] = function()
  local value = math.format(32500.25, 4, ',', '.')
  assert( value == '32.500,2500', value )
end

test['format 32500.25 to 32.500,25000'] = function()
  local value = math.format(32500.25, 5, ',', '.')
  assert( value == '32.500,25000', value )
end

test['format 32500.25 to 32.500,250000'] = function()
  local value = math.format(32500.25, 6, ',', '.')
  assert( value == '32.500,250000', value )
end

test['format 332500.25 to 332.500,25'] = function()
  local value = math.format(332500.25, 2, ',', '.')
  assert( value == '332.500,25', value )
end

test['format 4332500.25 to 4.332.500,25'] = function()
  local value = math.format(4332500.25, 2, ',', '.')
  assert( value == '4.332.500,25', value )
end

test['format 54332500.25 to 54.332.500,25'] = function()
  local value = math.format(54332500.25, 2, ',', '.')
  assert( value == '54.332.500,25', value )
end

test['format 654332500.25 to 654.332.500,25'] = function()
  local value = math.format(654332500.25, 2, ',', '.')
  assert( value == '654.332.500,25', value )
end

test['format 7654332500.25 to 7.654.332.500,25'] = function()
  local value = math.format(7654332500.25, 2, ',', '.')
  assert( value == '7.654.332.500,25', value )
end

test['format 87654332500.25 to 87.654.332.500,25'] = function()
  local value = math.format(87654332500.25, 2, ',', '.')
  assert( value == '87.654.332.500,25', value )
end

test['format 987654332500.25 to 987.654.332.500,25'] = function()
  local value = math.format(987654332500.25, 2, ',', '.')
  assert( value == '987.654.332.500,25', value )
end

test['format 987654332500 to 987.654.332.500'] = function()
  local value = math.format(987654332500.25, 0, ',', '.')
  assert( value == '987.654.332.500', value )
end

test['format 87654332500 to 87.654.332.500'] = function()
  local value = math.format(87654332500.25, 0, ',', '.')
  assert( value == '87.654.332.500', value )
end

test['format 7654332500.25 to 7.654.332.500'] = function()
  local value = math.format(7654332500.25, 0, ',', '.')
  assert( value == '7.654.332.500', value )
end

test['format 654332500.25 to 654.332.500'] = function()
  local value = math.format(654332500.25, 0, ',', '.')
  assert( value == '654.332.500', value )
end

test['format 54332500.25 to 54.332.500'] = function()
  local value = math.format(54332500.25, 0, ',', '.')
  assert( value == '54.332.500', value )
end

test['format 4332500.25 to 4.332.500'] = function()
  local value = math.format(4332500.25, 0, ',', '.')
  assert( value == '4.332.500', value )
end

test['format 4332500.25 to 4.332.500'] = function()
  local value = math.format(4332500.25, 0, ',', '.')
  assert( value == '4.332.500', value )
end

test['format 332500.25 to 332.500'] = function()
  local value = math.format(332500.25, 0, ',', '.')
  assert( value == '332.500', value )
end

test['format 32500.25 to 32.500'] = function()
  local value = math.format(32500.25, 0, ',', '.')
  assert( value == '32.500', value )
end

test['format 2500.25 to 2.500'] = function()
  local value = math.format(2500.25, 0, ',', '.')
  assert( value == '2.500', value )
end

test['format 500.25 to 500'] = function()
  local value = math.format(500.25, 0, ',', '.')
  assert( value == '500', value )
end

test['format 00.25 to 0'] = function()
  local value = math.format(00.25, 0, ',', '.')
  assert( value == '0', value )
end

test['format 0.25 to 0'] = function()
  local value = math.format(0.25, 0, ',', '.')
  assert( value == '0', value )
end

test['format 0.50 to 1'] = function()
  local value = math.format(0.50, 0, ',', '.')
  assert( value == '1', value )
end

test['format 0.5 to 0,50'] = function()
  local value = math.format(0.50, 2, ',', '.')
  assert( value == '0,50', value )
end

test['format 1234.57 to 1,234.57'] = function()
  local value = math.format(1234.57, 2, '.', ',')
  assert( value == '1,234.57', value )
end

test['format -1234.57 to -1,234.57'] = function()
  local value = math.format(-1234.57, 2, '.', ',')
  assert( value == '-1,234.57', value )
end

test['format 12346578.00 to 12,346,578.00'] = function()
  local value = math.format(12346578.00, 2, '.', ',')
  assert( value == '12,346,578.00', value )
end

test['format -12346578.00 to -12,346,578.00'] = function()
  local value = math.format(-12346578.00, 2, '.', ',')
  assert( value == '-12,346,578.00', value )
end

test['format 305450479 to 305,450,479.00'] = function()
  local value = math.format(305450479, 2, '.', ',')
  assert( value == '305,450,479.00', value )
end

test['format 402653183.00000 to 402,653,183.00'] = function()
  local value = math.format(402653183.00000, 2, '.', ',')
  assert( value == '402,653,183.00', value )
end

test['format 123456789.00 to 123,456,789.00'] = function()
  local value = math.format(123456789.00, 2, '.', ',')
  assert( value == '123,456,789.00', value )
end

test['format 123.455 to 123.46'] = function()
  local value = math.format(123.455, 2, '.', ',')
  assert( value == '123.46', value )
end

test['format 123.46 to 123.46'] = function()
  local value = math.format(123.46, 2, '.', ',')
  assert( value == '123.46', value )
end

test['format 0 to 0.0'] = function()
  local value = math.format(0, 1, '.', ',')
  assert( value == '0.0', value )
end

test['format 0 to 0.00'] = function()
  local value = math.format(0, 2, '.', ',')
  assert( value == '0.00', value )
end

test['format 0 to 0,00'] = function()
  local value = math.format(0, 2, ',', '')
  assert( value == '0,00', value )
end

test['format 0 to 0,0'] = function()
  local value = math.format(0, 1, ',', '')
  assert( value == '0,0', value )
end

test['format 1234.57 to 1 234.57'] = function()
  local value = math.format(1234.57, 2, '.', ' ')
  assert( value == '1 234.57', value )
end

test['format 1234.57 to -1 234.57'] = function()
  local value = math.format(-1234.57, 2, '.', ' ')
  assert( value == '-1 234.57', value )
end

test['format 12346578.00 to 12 346 578.00'] = function()
  local value = math.format(12346578.00, 2, '.', ' ')
  assert( value == '12 346 578.00', value )
end

test['format -12346578.00 to -12 346 578.00'] = function()
  local value = math.format(-12346578.00, 2, '.', ' ')
  assert( value == '-12 346 578.00', value )
end

test['format 12346578.90 to 12 346 578.90'] = function()
  local value = math.format(12346578.90, 2, '.', ' ')
  assert( value == '12 346 578.90', value )
end

test['format -12346578.90 to -12 346 578.90'] = function()
  local value = math.format(-12346578.90, 2, '.', ' ')
  assert( value == '-12 346 578.90', value )
end

test['format 305450479.00 to 305 450 479.00'] = function()
  local value = math.format(305450479.00, 2, '.', ' ')
  assert( value == '305 450 479.00', value )
end

test['format 402653183.00 to 402 653 183.00'] = function()
  local value = math.format(402653183.00, 2, '.', ' ')
  assert( value == '402 653 183.00', value )
end

test['format 123456.789 to 123456.789'] = function()
  local value = math.format(123456.789, 3, '.', '')
  assert( value == '123456.789', value )
end

test['format 123456.789 to 123456.789 without decimal point'] = function()
  local value = math.format(123456.789, 3, '', '')
  assert( value == '123456.789', value )
end

test['format 0.0 to 0'] = function()
  local value = math.format(0.0, 0, '', '')
  assert( value == '0', value )
end

test['format 0.0 to 0.0'] = function()
  local value = math.format(0.0, 1, '', '')
  assert( value == '0.0', value )
end

test['format 0 to 0.00'] = function()
  local value = math.format(0, 2, '', '')
  assert( value == '0.00', value )
end

test['format 1 to 1'] = function()
  local value = math.format(1, 0, '', '')
  assert( value == '1', value )
end

test['format 1 to 1.0'] = function()
  local value = math.format(1, 1, '', '')
  assert( value == '1.0', value )
end

test['format 305450479 to 305,450,479'] = function()
  local value = math.format(305450479, 0, '', ',')
  assert( value == '305,450,479', value )
end

test['format 1234.56 to 1,234.56'] = function()
  local value = math.format(1234.56, 2, '.', ',')
  assert( value == '1,234.56', value )
end

test['format -305450479.0 to -305,450,479*00'] = function()
  local value = math.format(-305450479.0, 2, '*', ',')
  assert( value == '-305,450,479*00', value )
end

test['format -305450479.567 to -305,450,479*567'] = function()
  local value = math.format(-305450479.567, 3, '*', ',')
  assert( value == '-305,450,479*567', value )
end

test['format 123456.012345678901 to 123.456,012345678901'] = function()
  local value = math.format(123456.012345678901, 12, ',', '.')
  assert( value == '123.456,012345678901', value )
end

test['format -123456.012345678901 to -123.456,012345678901'] = function()
  local value = math.format(-123456.012345678901, 12, ',', '.')
  assert( value == '-123.456,012345678901', value )
end

test['format 1.000000000000001 to 1,000000000000001'] = function()
  local value = math.format(1.000000000000001, 15, ',', '.')
  assert( value == '1,000000000000001', value )
end

test['format -1.000000000000001 to -1,000000000000001'] = function()
  local value = math.format(-1.000000000000001, 15, ',', '.')
  assert( value == '-1,000000000000001', value )
end

test['format 1e80'] = function()
  local value = math.format(1e80, 2, ',', '.')
  assert( value == '100.000.000.000.000.000.026.609.864.708.367.276.537.402.401.181.200.809.098.131.977.453.489.758.916.313.088,00', value )
end

test['format 1e300'] = function()
  local value = math.format(1e300, 2, ',', '.')
  assert( value == '1.000.000.000.000.000.052.504.760.255.204.420.248.704.468.581.108.159.154.915.854.115.511.802.457.988.908.195.786.371.375.080.447.864.043.704.443.832.883.878.176.942.523.235.360.430.575.644.792.184.786.706.982.848.387.200.926.575.803.737.830.233.794.788.090.059.368.953.234.970.799.945.081.119.038.967.640.880.074.652.742.780.142.494.579.258.788.820.056.842.838.115.669.472.196.386.865.459.400.540.160,00', value )
end

test['format 1e320'] = function()
  local value = math.format(1e300, 2, ',', '.')
  assert( value == '1.000.000.000.000.000.052.504.760.255.204.420.248.704.468.581.108.159.154.915.854.115.511.802.457.988.908.195.786.371.375.080.447.864.043.704.443.832.883.878.176.942.523.235.360.430.575.644.792.184.786.706.982.848.387.200.926.575.803.737.830.233.794.788.090.059.368.953.234.970.799.945.081.119.038.967.640.880.074.652.742.780.142.494.579.258.788.820.056.842.838.115.669.472.196.386.865.459.400.540.160,00', value )
end

test['format 1e1000'] = function()
  local value = math.format(1e300, 2, ',', '.')
  assert( value == '1.000.000.000.000.000.052.504.760.255.204.420.248.704.468.581.108.159.154.915.854.115.511.802.457.988.908.195.786.371.375.080.447.864.043.704.443.832.883.878.176.942.523.235.360.430.575.644.792.184.786.706.982.848.387.200.926.575.803.737.830.233.794.788.090.059.368.953.234.970.799.945.081.119.038.967.640.880.074.652.742.780.142.494.579.258.788.820.056.842.838.115.669.472.196.386.865.459.400.540.160,00', value )
end

test['format 1e1000 not crashes with 10 decimals forces 2'] = function()
  local value = math.format(1e300, 10, ',', '.')
  assert( value == '1.000.000.000.000.000.052.504.760.255.204.420.248.704.468.581.108.159.154.915.854.115.511.802.457.988.908.195.786.371.375.080.447.864.043.704.443.832.883.878.176.942.523.235.360.430.575.644.792.184.786.706.982.848.387.200.926.575.803.737.830.233.794.788.090.059.368.953.234.970.799.945.081.119.038.967.640.880.074.652.742.780.142.494.579.258.788.820.056.842.838.115.669.472.196.386.865.459.400.540.160,00', value )
end

return test
