# arken.net.WebSocket — exemplo de echo

O exemplo mais simples de WebSocket possível, mas mostrando algo que
prova que a mensagem foi até o servidor e voltou: cada resposta vem
embrulhada com o `sessionId` da conexão (um UUID gerado pelo servidor no
handshake) e um horário calculado do lado do servidor — informação que o
JavaScript do navegador não tem como inventar sozinho.

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
cd examples/arken.net.WebSocket/echo
mkdir -p tmp/pid
arken script/server
```

O servidor sobe em `ws://127.0.0.1:8091` (porta diferente do exemplo
`chat`, de propósito, pra rodar os dois ao mesmo tempo sem conflito).

## Endpoint

- `ws://127.0.0.1:8091/ws/echo` → `ws/controllers/EchoWebSocket.lua`

  Manda de volta o texto recebido, mas embrulhado em JSON com
  `sessionId` e `timestamp`:

  ```json
  {"event":"echo","sessionId":"6f88add0-...","timestamp":"14:32:07","payload":"oi"}
  ```

  (frame binário é a exceção: ecoa cru, sem embrulhar em JSON — ver
  comentário em `EchoWebSocket:onMessage`)

## Testando manualmente

Com qualquer cliente WebSocket (ex: [websocat](https://github.com/vi/websocat)):

```sh
websocat ws://127.0.0.1:8091/ws/echo
```

## Página HTML (`/`)

`http://127.0.0.1:8091/` é um controller HTTP normal (não WebSocket) —
ver `app/controllers/IndexController.lua` e `app/views/index/index.html`
— que serve uma página com uma caixa de texto e uma área de log, em
HTML/JS puro, sem lib externa. O JavaScript:

- abre a conexão (`new WebSocket(...)`) com `ws://127.0.0.1:8091/ws/echo`;
- manda o que for digitado no formulário;
- recebe a resposta, faz `JSON.parse`, e monta a linha do log com
  `sessionId`/`timestamp` **só se esses campos vierem** na mensagem (não
  assume que vão sempre existir - ver `prefixoInfo()` no JS da página);
- não faz eco local (diferente do exemplo `chat`) - o objetivo aqui é
  justamente esperar a resposta do servidor pra confirmar visualmente
  que o texto fez o percurso todo.
