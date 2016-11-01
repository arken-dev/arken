require "OHttpParser"
local should = require "test.should"
local test   = {}

test['deve retornar path /pedido/varejo'] = function()
  local header  = os.read(OBERON_PATH .. '/specs/OHttpParser/example-1.header')
  local request = OHttpParser.new(header)
  json = [[{
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
    "Accept-Encoding": "gzip, deflate",
    "Accept-Language": "pt-BR,pt;q=0.8,en-US;q=0.5,en;q=0.3",
    "Connection": "keep-alive",
    "Cookie": "__gads=ID=65db397af496b42f:T=1459784587:S=ALNI_MaXlE55QEou3h9txkA8jA5kPzMKVg; __utma=111872281.1750660672.1459784737.1459784737.1459784737.1; __utmz=111872281.1459784737.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none)",
    "Host": "localhost:3000",
    "User-Agent": "Mozilla/5.0 (X11; Linux x86_64; rv:38.0)",
    "fragment": "",
    "headerDone": "",
    "httpVersion": "HTTP/1.1",
    "queryString": "id=1234&descricao=teste",
    "requestMethod": "GET",
    "requestPath": "/pedido/varejo",
    "requestUri": "/pedido/varejo?id=1234&descricao=teste"
}
]]
  assert(request:toJson() == json)
end

return test
