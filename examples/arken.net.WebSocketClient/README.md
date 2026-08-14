# arken.net.WebSocketClient — exemplos

> **Precisa dos servidores dos exemplos `arken.net.WebSocket/echo` e/ou
> `arken.net.WebSocket/chat` rodando**, num build configurado com
> `-DARKEN_NET_HTTPSERVER=libev-ws` (o backend padrão, `libev`, não
> entende WebSocket). Ver "Pré-requisito" de cada exemplo abaixo.

Scripts standalone (não são apps HTTP) que conectam como *cliente* nos
servidores dos exemplos `arken.net.WebSocket`.

## echo.lua

Conecta no servidor do exemplo `arken.net.WebSocket/echo`, manda algumas
mensagens e mostra o `sessionId`/`timestamp` que o servidor devolve —
provando que o texto realmente foi até lá e voltou, não é eco local.

### Pré-requisito

O servidor do exemplo `arken.net.WebSocket/echo` rodando em
`ws://127.0.0.1:8091` (precisa do backend `libev-ws` — ver o README
desse outro exemplo pra como configurar o CMake e subir o servidor).

### Como rodar

A partir da raiz do repositório, com o servidor do echo já no ar:

```sh
bin/arken examples/arken.net.WebSocketClient/echo.lua
```

Saída esperada:

```
[client] conexão WebSocket estabelecida
[client] servidor confirmou sessão 6f88add0-...
[client] eco de 6f88add0-... às 14:32:07: oi
[client] eco de 6f88add0-... às 14:32:07: tudo bem?
[client] eco de 6f88add0-... às 14:32:07: última mensagem
[client] fechado (code=1000, reason="fim do exemplo")
```

## chat_bob.lua + chat_alice.lua

Dois scripts separados que conectam na mesma sala do exemplo
`arken.net.WebSocket/chat` e trocam mensagens de verdade entre si - um
chat real entre dois processos, não simulado. Bob conecta e fica
esperando (reativo); Alice conecta e inicia a conversa; cada um responde
ao que recebe do outro, num vaivém roteirizado (sem interatividade -
ler do teclado bloquearia `loop()`, ver comentário em cada script), até
os dois se desconectarem sozinhos depois da última fala.

### Pré-requisito

O servidor do exemplo `arken.net.WebSocket/chat` rodando em
`ws://127.0.0.1:8090` (precisa do backend `libev-ws` — ver o README
desse outro exemplo).

### Como rodar

A partir da raiz do repositório, com o servidor do chat já no ar, em
**dois terminais diferentes** - Bob primeiro, Alice logo depois:

```sh
# terminal 1
bin/arken examples/arken.net.WebSocketClient/chat_bob.lua
```

```sh
# terminal 2 (poucos segundos depois)
bin/arken examples/arken.net.WebSocketClient/chat_alice.lua
```

Saída esperada no terminal do Bob:

```
[Bob] entrei na sala, esperando Alice
[Bob] <sessionId-bob> entrou na sala
[Bob] <sessionId-alice> entrou na sala
[Bob] recebi: <sessionId-alice>#14:32:07: oi Bob! tudo bem?
[Bob] recebi: <sessionId-alice>#14:32:07: que bom que já tava aqui
[Bob] recebi: <sessionId-alice>#14:32:07: beleza, até mais!
[Bob] fechado (code=1000, reason="fim do exemplo")
```

## O que esses exemplos mostram

- `client.onOpen`/`onMessage`/`onClose`/`onError` atribuídos direto no
  objeto (igual `ws.onopen = ...` do JavaScript), sem precisar de um
  módulo dispatcher separado.
- `client:connect()` não bloqueia - a conexão acontece numa thread
  própria, em paralelo.
- `while client:loop() do end` - só é seguro chamar Lua de volta na
  mesma thread que atribuiu os callbacks (ver comentário de
  `arken::net::WebSocketClient` no header), então `loop()` é o ponto
  onde os eventos pendentes (open/message/close/error) são de fato
  entregues. Volta `false` sozinho quando a conexão fecha, terminando o
  `while`.
- Fechar a conexão de dentro de um callback (`self:close(...)` chamado
  de dentro de `onMessage`) funciona normalmente - inclusive mandar uma
  última mensagem e fechar em seguida (`self:send(...)` + `self:close(...)`
  na mesma chamada): a mensagem final sempre sai antes da conexão
  fechar de verdade (ver `chat_bob.lua`/`chat_alice.lua`, que fazem
  exatamente isso na última fala).
