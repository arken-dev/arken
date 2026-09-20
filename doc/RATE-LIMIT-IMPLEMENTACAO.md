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

## `include/arken/security/ratelimit.h`

```cpp
// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_SECURITY_RATELIMIT_
#define _ARKEN_SECURITY_RATELIMIT_

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

namespace arken {
namespace security {

// janela fixa: no primeiro hit do IP a janela abre; depois de
// minutes * 60 segundos os hits zeram e uma nova janela começa.
// Não é token bucket.
class RateLimit
{
  public:
  RateLimit(unsigned limit, unsigned minutes = 1);
  ~RateLimit();

  // incrementa o IP; true = estourou o limite (avisar a borda)
  bool count(const char * ip);
  void clear(const char * ip);
  void gc(unsigned idle_seconds);

  private:
  struct Record {
    unsigned hits = 0;
    std::chrono::steady_clock::time_point window_start{};
    std::chrono::steady_clock::time_point last{};
  };

  unsigned m_limit;
  std::chrono::seconds m_window;
  std::mutex m_mutex;
  std::unordered_map<std::string, Record> m_map;
};

} // namespace security
} // namespace arken

#endif
```

Janela **fixa**: no primeiro hit abre-se a janela; depois de `minutes * 60` segundos os hits zeram e começa outra. Não é token bucket.

---

## `src/arken/security/ratelimit.cpp`

```cpp
// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/security/ratelimit.h>

namespace arken {
namespace security {

using clock = std::chrono::steady_clock;

RateLimit::RateLimit(unsigned limit, unsigned minutes)
  : m_limit(limit)
  , m_window(static_cast<long>(minutes == 0 ? 1 : minutes) * 60)
{
}

RateLimit::~RateLimit() = default;

bool RateLimit::count(const char * ip)
{
  if( ip == nullptr || ip[0] == '\0' ) {
    return false;
  }

  const auto now = clock::now();
  std::lock_guard<std::mutex> lock(m_mutex);
  auto & r = m_map[ip];

  if( r.window_start.time_since_epoch().count() == 0 ||
      now - r.window_start >= m_window ) {
    r.hits = 0;
    r.window_start = now;
  }

  r.hits += 1;
  r.last  = now;
  return r.hits >= m_limit;
}

void RateLimit::clear(const char * ip)
{
  if( ip == nullptr ) {
    return;
  }
  std::lock_guard<std::mutex> lock(m_mutex);
  m_map.erase(ip);
}

void RateLimit::gc(unsigned idle_seconds)
{
  const auto now  = clock::now();
  const auto idle = std::chrono::seconds(idle_seconds);
  std::lock_guard<std::mutex> lock(m_mutex);
  for( auto it = m_map.begin(); it != m_map.end(); ) {
    if( now - it->second.last > idle ) {
      it = m_map.erase(it);
    } else {
      ++it;
    }
  }
}

} // namespace security
} // namespace arken
```

---

## Binding Lua (padrão Zip)

Mesmo esquema de `src/bindings/arken/compress/zip/zip.cpp`: userdata
`RateLimit**`, `luaL_reg`, `luaopen_arken_security_RateLimit`. Código real
em `src/bindings/arken/security/ratelimit/ratelimit.cpp` — não duplicado
aqui pra não divergir da implementação.

API Lua:

```lua
local RateLimit = require('arken.security.RateLimit')
local rl = RateLimit.new(30, 1)  -- 30 hits / 1 minuto

rl:count(ip)   -- boolean: true = estourou → borda
rl:clear(ip)   -- login OK
rl:gc(3600)    -- apaga IPs ociosos (chamar de timer)
```

Uma instância por processo (módulo, não por request):

```lua
-- lib/Security/RateLimit.lua  (alias opcional)
local RateLimit = require('arken.security.RateLimit')
return RateLimit.new(30, 1)
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
- [ ] `gc` periódico (ex. 1 h de ociosidade)
- [ ] `:count == true` → Lists API
- [ ] Escada 5/5 min só no `Usuario.login`
- [ ] Começar com `new(30, 1)`; descer para 25 e 20 com base no log
