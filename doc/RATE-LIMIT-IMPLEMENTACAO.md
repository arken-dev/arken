# Rate limit de acesso inválido — implementação

Documento da **camada 3** (aplicação). As outras camadas não entram no código abaixo.

| Camada | Papel |
|---|---|
| WAF (Cloudflare) | teto grosso por IP, cache, túnel, `$blocklist` |
| fail2ban | lê `429`/`401`/`404` no log e grava na **mesma** `$blocklist` |
| Aplicação | conta evento ruim por IP; estourou X/min → avisa a borda |

## Regra

Dois relógios **separados**:

1. **C++ `RateLimit`** — janela de N minutos, só soma. `true` = avisar a Cloudflare.
2. **Lua `Usuario.login`** — escada 5 erros → espera 5 min (depois 10, 15… teto 60). Isso **não** vive no C++.

Evento ruim (sempre chama `:count(ip)`):

- `POST` login com senha errada
- `POST` login **durante** a espera de 5 min (a app nem valida senha; mesmo assim conta)
- acesso a path que exige sessão, **sem** sessão

Não conta: GET da tela de login, path público, request **com** sessão válida, webhook.

Limite inicial: **30 / 1 min**. Depois 25, depois **20**, quando o log mostrar que 30 não pega abuso de verdade.

Login OK → `:clear(ip)`.

Sinal `true` → Block na `$blocklist` em `.app` e `.app.br` (neste sinal não usamos challenge).

Header do IP: `CF-Connecting-IP` (túnel). Não usar peer TCP.

---

## Arquivos no Arken

```text
include/arken/security/ratelimit.h
src/arken/security/ratelimit.cpp
src/bindings/arken/security/ratelimit/ratelimit.cpp
src/bindings/arken/security/ratelimit/CMakeLists.txt
src/bindings/arken/security/CMakeLists.txt

tests/lib/arken/security/RateLimit/new.lua
tests/lib/arken/security/RateLimit/count.lua
tests/lib/arken/security/RateLimit/clear.lua
tests/lib/arken/security/RateLimit/gc.lua
tests/lib/arken/security/RateLimit/size.lua

examples/arken.security.ratelimit/basic-usage.lua
examples/arken.security.ratelimit/controller.lua
```

Registro no build (mesmo esquema de `arken.compress.Zip`):

- `src/arken/CMakeLists.txt`: glob `security/*.cpp` somado à lib `arken`.
- `src/bindings/arken/CMakeLists.txt`: `add_subdirectory(security)`.

`require` Lua: `arken.security.RateLimit`
Alias opcional da app: `Security.RateLimit`.

Namespace `arken.security` é novo no repo (não existia `arken.security.*`
antes). Segue a mesma convenção dos módulos existentes (`arken.compress`,
`arken.digest`, `arken.cache`, ...): namespace em minúsculo, classe em
PascalCase.

---

## `include/arken/security/ratelimit.h` / `src/arken/security/ratelimit.cpp`

Não duplicado aqui (já divergiu do código real uma vez — ver histórico
do namespace, `env():get` vs `env():field()`). Código-fonte é a
referência; resumo do que importa:

- Janela **fixa**: no primeiro hit do IP a janela abre; depois de
  `seconds` segundos os hits zeram e começa outra. Não é token bucket.
  Parâmetro em segundos (não minutos) — quem quiser 1 minuto passa `60`
  (default do construtor).

- **Faxina automática, sem thread própria.** `count()` nunca removia
  entradas sozinho — só reseta a janela de um IP que *volta* a aparecer.
  Um IP visto uma única vez (comum: milhares de IPs distintos por dia)
  ficava em `m_map` pra sempre, sem `gc()` explícito. A correção: a cada
  `2×window` de tempo real decorrido, `count()` varre e descarta (sob o
  mesmo lock que já segura) os IPs ociosos há mais de `2×window`,
  pegando carona no tráfego normal.

  Cogitou-se copiar o padrão de `arken::cache` (thread de background via
  `std::thread(...).detach()`), mas esse padrão só é seguro lá porque os
  buckets do cache são singletons que nunca são destruídos. Uma
  `RateLimit` é criada/destruída pelo app (`__gc` do binding chama
  `delete`) — uma thread detached capturando `this` ficaria com ponteiro
  pendurado assim que a instância fosse liberada. Por isso a faxina é
  amortizada dentro do próprio `count()`, sem thread nova.

- `size()` — número de IPs atualmente rastreados. Existe pra
  introspecção/monitoramento e pra provar em teste que a faxina (manual
  via `gc()` ou automática) realmente encolhe `m_map` — ver
  `tests/lib/arken/security/RateLimit/size.lua`.

---

## Binding Lua (padrão Zip)

Mesmo esquema de `src/bindings/arken/compress/zip/zip.cpp`: userdata
`RateLimit**`, `luaL_reg`, `luaopen_arken_security_RateLimit`. Código real
em `src/bindings/arken/security/ratelimit/ratelimit.cpp` — não duplicado
aqui pra não divergir da implementação.

API Lua:

```lua
local RateLimit = require('arken.security.RateLimit')
local rl = RateLimit.new(30, 60)  -- 30 hits / 60 segundos

rl:count(ip)   -- boolean: true = estourou → borda
rl:clear(ip)   -- login OK
rl:gc(3600)    -- apaga IPs ociosos manualmente (opcional: count() já faxina sozinho)
rl:size()      -- quantos IPs estão rastreados agora (introspecção/monitoramento)
```

Uma instância por processo (módulo, não por request):

```lua
-- lib/Security/RateLimit.lua  (alias opcional)
local RateLimit = require('arken.security.RateLimit')
return RateLimit.new(30, 60)
```

---

## Controller

Exemplo completo e testável em `examples/arken.security.ratelimit/controller.lua`
(roda sem precisar de servidor HTTP de verdade — mocka `_env` do jeito que
`tests/lib/arken/Controller/*.lua` já fazem).

Ponto importante: a API real de `arken.net.HttpEnv` (ver
`lib/arken/net/HttpRequest.lua`) é `env():field(nome)`, **não**
`env():get(nome)`.

```lua
local MyController = Class.new("MyController", "Controller")
local bad = require('Security.RateLimit') -- instância única

local function clientIp(self)
  return self:env():field('CF-Connecting-IP')
end

function MyController:loginAction()
  local ip = clientIp(self)
  local ok = pcall(Usuario.login, self:params().login, self:params().senha)

  if ok then
    bad:clear(ip)
    return
  end

  -- senha errada OU tentou dentro da espera de 5 min
  if bad:count(ip) then
    Cloudflare.block(ip) -- POST na IP List $blocklist
  end
end
```

Path que exige sessão, sem estar logado:

```lua
function MyController:algumaActionProtegida()
  if self:session().user then
    return -- fluxo normal
  end

  local ip = clientIp(self)
  if bad:count(ip) then
    Cloudflare.block(ip)
  end
  -- 401; mensagem genérica
end
```

`Usuario.login` (camada Lua, **não** o C++):

- 5 falhas → não valida senha pelos próximos 5 min
- nessa espera cada `pcall` falha de propósito e o controller ainda chama `bad:count`
- mais 5 na espera → 10 min, etc., teto 60 min
- quem espera quieto: a janela de 60 s do C++ esvazia sozinha

---

## Cloudflare (o `true` do `:count`)

1. Account → Lists → IP list `blocklist`
2. Security rule: `ip.src in $blocklist` → **Block** (os dois hosts)
3. Token: **Account Filter Lists: Edit**
4. `Cloudflare.block(ip)` = `POST /accounts/{id}/rules/lists/{list_id}/items` com `[{"ip":"...","comment":"app-ratelimit"}]`

A app e o fail2ban usam **a mesma lista**. Comece mandando à borda **só** quando `:count` for `true`. Fail2ban cobre reincidência via log.

---

## Checklist

- [ ] Header `CF-Connecting-IP` no log e no `:count`
- [ ] Instância única de `RateLimit` no processo
- [ ] `:count` só em evento ruim
- [ ] `:clear` no login OK
- [x] faxina automática (dentro do `:count`, sem precisar de timer externo) — `gc()` manual continua disponível, mas é opcional
- [ ] `:count == true` → Lists API
- [ ] Escada 5/5 min só no `Usuario.login`
- [ ] Começar com `new(30, 60)`; descer para 25 e 20 com base no log
