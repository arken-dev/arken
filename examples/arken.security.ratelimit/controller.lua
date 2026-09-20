-- arken.security.RateLimit example: integrando num Controller
--
-- Padrão real de uso: uma única instância de RateLimit por processo
-- (não por request), count() só em evento ruim, clear() no login OK, e o
-- IP lido via env():field('CF-Connecting-IP') -- não existe env():get()
-- na API do Arken (ver lib/arken/net/HttpRequest.lua).
--
-- Este exemplo não sobe um HttpServer de verdade: monta um "_env" mockado
-- do mesmo jeito que tests/lib/arken/Controller/*.lua fazem, e injeta os
-- params já prontos via `_params` (Controller:params() só recalcula
-- quando self._params ainda é nil).

require('arken.Controller') -- registra a classe "Controller" em Class.lookup

local RateLimit = require('arken.security.RateLimit')

-- instância única do processo
local badLogins = RateLimit.new(3, 1) -- 3 eventos ruins / 1 minuto

local blockedIps = {}
local function CloudflareBlock(ip)
  blockedIps[ip] = true
  print(string.format('  [Cloudflare.block] %s adicionado a $blocklist', ip))
end

local MyController = Class.new("MyController", "Controller")

local function clientIp(self)
  return self:env():field('CF-Connecting-IP')
end

function MyController:loginAction()
  local ip = clientIp(self)
  local ok = self:params().senha == 'senha-correta' -- no app real: pcall(Usuario.login, ...)

  if ok then
    badLogins:clear(ip)
    return 200, {}, 'login ok'
  end

  -- senha errada: conta contra o IP; true = avisar a borda
  if badLogins:count(ip) then
    CloudflareBlock(ip)
  end

  return 401, {}, 'login invalido'
end

function MyController:protectedAction()
  if self:session().user then
    return 200, {}, 'ok, usuario logado'
  end

  -- path que exige sessão, sem sessão: também conta como evento ruim
  local ip = clientIp(self)
  if badLogins:count(ip) then
    CloudflareBlock(ip)
  end

  return 401, {}, 'sessao necessaria'
end

-------------------------------------------------------------------------------
-- simulando requests (mock de env, sem HttpServer de verdade)
-------------------------------------------------------------------------------

local function mockEnv(ip)
  local env = {}
  env.field = function(_, name)
    if name == 'CF-Connecting-IP' then return ip end
    return '' -- inclusive Cookie: sem sessão (env real nunca devolve nil, e sim "")
  end
  return env
end

local attackerIp = '198.51.100.20'

print('-- tentativas de login com senha errada --')
for i = 1, 4 do
  local c = MyController.new{ _env = mockEnv(attackerIp), _params = { senha = 'senha-errada' } }
  local code = c:loginAction()
  print(string.format('  tentativa %d -> %d (bloqueado=%s)', i, code, tostring(blockedIps[attackerIp] == true)))
end

print('-- login correto limpa o contador --')
do
  local c = MyController.new{ _env = mockEnv(attackerIp), _params = { senha = 'senha-correta' } }
  local code = c:loginAction()
  print(string.format('  login ok -> %d', code))
end

print('-- acesso a path protegido sem sessao --')
do
  local c = MyController.new{ _env = mockEnv('203.0.113.55') }
  local code = c:protectedAction()
  print(string.format('  sem sessao -> %d', code))
end

print('arken.security.RateLimit controller OK')
