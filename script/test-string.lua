print("test using", arg[1])

local buffer = os.read(arg[1])
print("#buffer", #buffer)

for i = 1, 10 do
print("test camelCase")
buffer:camelCase()
collectgarbage("collect")

print("test capitalize")
buffer:capitalize()
collectgarbage("collect")

print("test center")
buffer:center(40, "=")
collectgarbage("collect")

print("test contains")
buffer:contains("=")
collectgarbage("collect")

print("test chop")
buffer:chop(40)
collectgarbage("collect")

print("test count")
buffer:count('rato')
collectgarbage("collect")

print("test dasherize")
buffer:dasherize()
collectgarbage("collect")

--print("encode64/decode64")
--buffer:encode64():decode64()

print("test escape")
buffer:escape()
collectgarbage("collect")

print("test escapeHtml")
buffer:escapeHtml()
collectgarbage("collect")

print("test indexOf")
buffer:indexOf('rato')
collectgarbage("collect")

print("test insert")
buffer:insert(1000, 'rato')
collectgarbage("collect")

print("test endsWith")
buffer:endsWith('rato')
collectgarbage("collect")

print("test indexLastOf")
buffer:lastIndexOf('rato')
collectgarbage("collect")

print("test left")
buffer:left(1000)
collectgarbage("collect")

print("test leftJustified")
buffer:leftJustified(1000, '-')
collectgarbage("collect")

print("test mid")
buffer:mid(1000)
collectgarbage("collect")

print("test md5")
print(buffer:md5())
collectgarbage("collect")

print("test normalize")
buffer:normalize()
collectgarbage("collect")

print("test simplified ")
buffer:simplified()
collectgarbage("collect")

print("test replace")
buffer:replace("gato", "rato")
buffer:replace("g", "r")
collectgarbage("collect")

print("test right")
buffer:right(1000)
collectgarbage("collect")

print("test rightJustified")
buffer:right(1000, '-')
collectgarbage("collect")

print("test sha1")
print(buffer:sha1())
collectgarbage("collect")

-- split

print("test suffix")
buffer:suffix("o rato")
collectgarbage("collect")

print("test startsWith")
print(buffer:startsWith("o rato"))
collectgarbage("collect")

print("test trimmed")
buffer:trimmed()
collectgarbage("collect")

print("test rightTrimmed")
buffer:rightTrimmed()
collectgarbage("collect")

print("test truncate")
buffer:truncate(100)
collectgarbage("collect")

print("test underscore")
buffer:underscore()
collectgarbage("collect")



end
