# arken.net.WebSocket — exemplo de chat

Aplicação completa mostrando o suporte a WebSocket do Arken: handshake,
roteamento por path, sessão por conexão, broadcast entre conexões
diferentes, e tratamento de erro/timeout via `error()`.

## Pré-requisito

Esse exemplo só funciona num build com o backend `libev-ws` (o backend
padrão, `libev`, usado em produção, não entende upgrade de WebSocket —
fica intocado de propósito). Configure o CMake com:

```sh
cmake -DARKEN_NET_HTTPSERVER=libev-ws ..
```

## Como rodar

A partir da raiz do repositório:

```sh
cd examples/arken.net.WebSocket/chat
mkdir -p tmp/pid
arken script/server
```

O servidor sobe em `ws://127.0.0.1:8090`.

## Endpoints

O path do handshake decide qual controller atende a conexão (ver
`lib/arken/net/websocketDispatcher.lua` — `/segmento/resto` vira
`segmento.controllers.Resto`, em PascalCase, com sufixo `WebSocket`):

- `ws://127.0.0.1:8090/ws/chat/room` → `ws/controllers/Chat/RoomWebSocket.lua`
  Sala de chat de verdade: quem entra/sai é anunciado pra todo mundo na
  sala (`WebSocketConnection.send()` pra cada `session_id` guardado em
  `arken.cache`), mensagens são retransmitidas pra todos, e 3 timeouts de
  ping seguidos fazem a aplicação encerrar a conexão (`error()` decide
  isso, não o framework).

## Testando manualmente

Com qualquer cliente WebSocket (ex: [websocat](https://github.com/vi/websocat),
ou o console do navegador):

```sh
websocat ws://127.0.0.1:8090/ws/chat/room
```

Abra `ws/chat/room` em duas abas/terminais diferentes pra ver o broadcast
funcionando entre as duas conexões.

## Página HTML (`/`)

`http://127.0.0.1:8090/` é um controller HTTP normal (não WebSocket) —
ver `app/controllers/IndexController.lua` e `app/views/index/index.html`
— que serve uma página com um chat funcional em HTML/JS puro, sem
nenhuma lib externa: o JavaScript da própria página abre a conexão
(`new WebSocket(...)`) com `ws://127.0.0.1:8090/ws/chat/room`, mostra as
mensagens recebidas em tela e manda o que for digitado no formulário.

Diferente do roteamento WebSocket (automático por convenção de path),
o roteamento HTTP normal precisa ser ligado explicitamente - ver
`server:setDispatcher("lib.dispatcher")` em `script/server` e
`lib/dispatcher.lua`.

Abra `http://127.0.0.1:8090/` em duas abas do navegador pra ver o
broadcast acontecendo entre elas.
