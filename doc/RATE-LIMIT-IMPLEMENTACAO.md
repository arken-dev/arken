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

Evento ruim (sempre chama `.count(ip)`):

- `POST` login com senha errada
- `POST` login **durante** a espera de 5 min (a app nem valida senha; mesmo assim conta)
- acesso a path que exige sessão, **sem** sessão

Não conta: GET da tela de login, path público, request **com** sessão válida, webhook.

Limite inicial: **30 / 1 min**. Depois 25, depois **20**, quando o log mostrar que 30 não pega abuso de verdade.

Login OK → `.clear(ip)`.

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

Namespace `arken.security` é novo no repo (não existia `arken.security.*`
antes). Segue a mesma convenção dos módulos existentes (`arken.compress`,
`arken.digest`, `arken.cache`, ...): namespace em minúsculo, classe em
PascalCase.

**Importante sobre o pool de VMs (`src/arken/mvm.cpp`):** o Arken roda um
*pool* de `lua_State*` independentes (`mvm::pop()`/`mvm::push()`), cada
um com seu próprio `luaL_newstate()` e portanto seu próprio
`package.loaded`. Um `local bad = require('Security.RateLimit')` que
devolvesse `RateLimit.new(limit, seconds)` só seria singleton *dentro de
uma VM* — cada VM do pool teria seu próprio contador, e um IP atacante
poderia nunca acumular hits suficiente numa VM só pra estourar o limite.
Por isso a API é nomeada (`RateLimit.new(name, ...)`, ver seção
"Binding Lua" abaixo): o registro que guarda os contadores é estático em
C++, fora de qualquer `lua_State`, e por isso é o mesmo em todas as VMs
do pool. Mesmo problema e mesma solução do `arken::cache`
(`cache.bucket(name)`).

---

## `include/arken/security/ratelimit.h` / `src/arken/security/ratelimit.cpp`

Não duplicado aqui (já divergiu do código real uma vez — ver histórico
do namespace, `env():get` vs `env():field()`). Código-fonte é a
referência; resumo do que importa:

- Janela **fixa**: no primeiro hit do IP a janela abre; depois de
  `seconds` segundos os hits zeram e começa outra. Não é token bucket.
  Parâmetro em segundos (não minutos) — quem quiser 1 minuto passa `60`
  (default do construtor).

- **Registro nomeado, igual `arken::cache::get(name)`.**
  `RateLimit::get(name, limit, seconds)` é estático: cria na primeira
  chamada com esse `name` e devolve a mesma instância nas chamadas
  seguintes (ignorando `limit`/`seconds` a partir da segunda). O registro
  (`s_registry`) é `static`/global em C++, então é o mesmo em todas as
  VMs do pool `mvm` — ver nota sobre o pool na seção anterior. Uma
  instância obtida por `get()` nunca é destruída (mesma premissa dos
  buckets do `cache`: vive pelo tempo de vida do processo).

- **Faxina automática por thread de background**, igual
  `arken::cache::backgroundGC()`/`ensureBackgroundGC()`. `count()` nunca
  removia entradas sozinho — só reseta a janela de um IP que *volta* a
  aparecer; um IP visto uma única vez (comum: milhares de IPs distintos
  por dia) ficava em `m_map` pra sempre, sem `gc()` explícito. A primeira
  chamada a `get()` agenda uma única thread compartilhada
  (`std::thread(...).detach()`) que, a cada poucos segundos, varre todas
  as instâncias registradas e descarta (via `gc()`) os IPs ociosos há
  mais de `2×window` de cada uma.

  Antes de existir o registro nomeado, uma `RateLimit` era criada e
  destruída livremente pelo app (`__gc` do binding chamava `delete`), e
  uma thread detached capturando `this` ficaria com ponteiro pendurado
  assim que a instância fosse liberada — por isso a primeira versão desse
  fix rodava a faxina amortizada dentro do próprio `count()`, sem thread.
  Isso deixou de ser necessário: como toda instância agora só existe via
  `get()` e nunca é destruída, o mesmo padrão de thread do `cache` passou
  a ser seguro.

- `size()` — número de IPs atualmente rastreados. Existe pra
  introspecção/monitoramento e pra provar em teste que a faxina (manual
  via `gc()` ou automática) realmente encolhe `m_map` — ver
  `tests/lib/arken/security/RateLimit/size.lua`.

---

## Binding Lua (padrão `cache.bucket`, não o padrão `Zip`)

A primeira versão desse binding copiava o padrão de
`src/bindings/arken/compress/zip/zip.cpp` (userdata dono + `__gc`
chamando `delete`) — adequado pra um objeto de uso local e descartável,
mas incompatível com um registro nomeado e compartilhado: se cada VM
tivesse seu próprio userdata "dono" do mesmo `RateLimit*`, o primeiro
`__gc` a rodar (em qualquer VM) daria `delete` no objeto compartilhado e
toda outra VM ainda segurando esse ponteiro quebraria.

O binding atual segue o mesmo esquema de
`cache.bucket(name)` (`src/bindings/arken/cache/cache.cpp`): sem
userdata, sem `__gc`. `RateLimit.new(name, limit, seconds)` devolve uma
tabela comum de *closures*, cada uma carregando `name`/`limit`/`seconds`
como upvalue e resolvendo `RateLimit::get(...)` a cada chamada. Como é
uma tabela comum (não um objeto com metatabela própria), as chamadas são
com **ponto, não dois-pontos**: `bad.count(ip)`, não `bad:count(ip)` — o
`:` empurraria a própria tabela como primeiro argumento no lugar do `ip`
real (mesma pegadinha existente em `cache.bucket(name).insert(...)`).

Código real em `src/bindings/arken/security/ratelimit/ratelimit.cpp` —
não duplicado aqui pra não divergir da implementação.

API Lua:

```lua
local RateLimit = require('arken.security.RateLimit')
local rl = RateLimit.new('bad-logins', 30, 60)  -- 30 hits / 60 segundos

rl.count(ip)   -- boolean: true = estourou → borda
rl.clear(ip)   -- login OK
rl.gc(3600)    -- apaga IPs ociosos manualmente (opcional: uma thread de
               -- background já faxina sozinha, ver seção anterior)
rl.size()      -- quantos IPs estão rastreados agora (introspecção/monitoramento)
```

`name` (aqui, `'bad-logins'`) é o que garante a mesma instância em
qualquer VM do pool `mvm` — qualquer arquivo, em qualquer VM, que chame
`RateLimit.new('bad-logins', ...)` cai no mesmo registro em C++. Não
precisa (e não ajuda) guardar isso atrás de um módulo Lua tipo
`require('Security.RateLimit')` esperando que o cache de `require()`
sirva de singleton — cada VM tem seu próprio `package.loaded`, então cada
uma chamaria `require` de novo de qualquer forma. Só repita a mesma
constante de nome onde for usar:

```lua
-- lib/Security/RateLimit.lua  (alias opcional, só por conveniência de nome)
local RateLimit = require('arken.security.RateLimit')
return RateLimit.new('bad-logins', 30, 60)
```

---

## Controller

Exemplo completo e testável em `examples/arken.security.ratelimit/controller.lua`
(roda sem precisar de servidor HTTP de verdade — mocka `_env` do jeito que
`tests/lib/arken/Controller/*.lua` já fazem).

Dois pontos importantes:

- A API real de `arken.net.HttpEnv` (ver `lib/arken/net/HttpRequest.lua`)
  é `env():field(nome)`, **não** `env():get(nome)`.
- `bad.count(ip)` / `bad.clear(ip)` são chamadas com **ponto**, não
  dois-pontos — `bad` é uma tabela de closures (ver seção "Binding
  Lua"), não um objeto com metatabela; `bad:count(ip)` empurraria `bad`
  como primeiro argumento no lugar do `ip`.

```lua
local MyController = Class.new("MyController", "Controller")
local bad = require('Security.RateLimit') -- mesmo nome em qualquer VM do pool

local function clientIp(self)
  return self:env():field('CF-Connecting-IP')
end

function MyController:loginAction()
  local ip = clientIp(self)
  local ok = pcall(Usuario.login, self:params().login, self:params().senha)

  if ok then
    bad.clear(ip)
    return
  end

  -- senha errada OU tentou dentro da espera de 5 min
  if bad.count(ip) then
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
  if bad.count(ip) then
    Cloudflare.block(ip)
  end
  -- 401; mensagem genérica
end
```

`Usuario.login` (camada Lua, **não** o C++):

- 5 falhas → não valida senha pelos próximos 5 min
- nessa espera cada `pcall` falha de propósito e o controller ainda chama `bad.count`
- mais 5 na espera → 10 min, etc., teto 60 min
- quem espera quieto: a janela de 60 s do C++ esvazia sozinha

---

## Cloudflare (o `true` do `.count`)

1. Account → Lists → IP list `blocklist`
2. Security rule: `ip.src in $blocklist` → **Block** (os dois hosts)
3. Token: **Account Filter Lists: Edit**
4. `Cloudflare.block(ip)` = `POST /accounts/{id}/rules/lists/{list_id}/items` com `[{"ip":"...","comment":"app-ratelimit"}]`

A app e o fail2ban usam **a mesma lista**. Comece mandando à borda **só** quando `.count` for `true`. Fail2ban cobre reincidência via log.

---

## Checklist

- [ ] Header `CF-Connecting-IP` no log e no `.count`
- [x] Mesmo `name` em todo lugar que usa `RateLimit.new(name, ...)` (garante o mesmo contador em qualquer VM do pool `mvm`, não só dentro de uma VM)
- [ ] `.count` só em evento ruim
- [ ] `.clear` no login OK
- [x] faxina automática (thread de background compartilhada, mesmo esquema do `arken::cache`) — `gc()` manual continua disponível, mas é opcional
- [ ] `.count == true` → Lists API
- [ ] Escada 5/5 min só no `Usuario.login`
- [ ] Começar com `new(name, 30, 60)`; descer para 25 e 20 com base no log
