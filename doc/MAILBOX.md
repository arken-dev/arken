Arken Concurrent Mailbox
1. Objetivo

Implementar uma nova primitiva de concorrência no Arken chamada Mailbox.

Namespace Lua:

arken.concurrent.mailbox

Uso:

local Mailbox = require("arken.concurrent.mailbox")

A Mailbox é uma estrutura nomeada de troca de mensagens entre produtores e consumidores.

Ela deve ser uma abstração generalista e não deve estar conceitualmente associada a VM → VM.

Pode ser utilizada para:

VM → VM;
thread → thread;
task → task;
worker → worker;
produtor → consumidor;
evento → processamento;
solução → tarefa;
qualquer outra comunicação interna entre unidades de execução.

A abstração deve ser:

Uma fila FIFO nomeada de mensagens, com suporte a produtores não bloqueantes e consumidores bloqueantes ou não bloqueantes.

2. Namespace

A implementação deve pertencer ao namespace:

arken.concurrent.mailbox

Não utilizar:

arken.mvm.mailbox

A existência de múltiplas VMs é apenas uma das possíveis utilizações da estrutura.

A Mailbox é uma primitiva de concorrência.

3. API pública

A API inicial deve ser deliberadamente pequena:

local Mailbox = require("arken.concurrent.mailbox")


local mailbox = Mailbox.open("marketplace")


mailbox:send(message)


local message = mailbox:receive()


local message = mailbox:fetch()


local closed = mailbox:isClosed()


mailbox:close()

A API oficial inicial possui somente:

Mailbox.open()
mailbox:send()
mailbox:receive()
mailbox:fetch()
mailbox:isClosed()
mailbox:close()

Não implementar inicialmente:

tryReceive()
poll()
onMessage()
loop()
subscribe()
unsubscribe()
empty()
size()
clear()

Essas operações podem ser consideradas futuramente, mas não fazem parte da primeira versão.

4. Mailbox.open()
Assinatura
local mailbox = Mailbox.open(name)

Exemplo:

local mailbox = Mailbox.open("marketplace")
Semântica

open() acessa uma Mailbox identificada pelo nome.

Se a Mailbox ainda não existir, ela deve ser criada.

Se já existir, uma referência para a Mailbox existente deve ser retornada.

Exemplo:

local a = Mailbox.open("marketplace")
local b = Mailbox.open("marketplace")

a e b podem ser objetos Lua diferentes, porém devem apontar para o mesmo estado interno compartilhado.

Conceitualmente:

                    Registry
                       │
                       ▼
              "marketplace"
                       │
                       ▼
               MailboxState
                       ▲
                       │
             ┌─────────┴─────────┐
             │                   │
          Lua VM A            Lua VM B
             │                   │
          mailbox A          mailbox B
5. Nome da Mailbox

O nome é o identificador lógico da Mailbox.

Exemplo:

Mailbox.open("marketplace")

O nome deve ser tratado como uma chave.

Exemplos possíveis:

marketplace
orders
payments
notifications
reconciliation
marketplace:1234
order:123456

A Mailbox não deve interpretar semanticamente o nome.

Para ela:

"marketplace"

é apenas uma chave.

6. send()
Assinatura
mailbox:send(message)
Semântica

Adiciona uma mensagem ao final da fila.

A operação deve ser não bloqueante em relação ao consumidor.

Exemplo:

mailbox:send({
    action = "order.updated",
    order_id = 1234
})

O produtor não deve esperar que outro consumidor processe a mensagem.

O fluxo conceitual é:

send(message)
      │
      ▼
   lock
      │
      ▼
queue.push(message)
      │
      ▼
   unlock
      │
      ▼
 notify_one()
      │
      ▼
   return

O objetivo é que send() seja uma operação curta.

7. FIFO

A Mailbox deve ser uma fila FIFO.

Se o produtor executar:

mailbox:send(message1)
mailbox:send(message2)
mailbox:send(message3)

o consumidor deverá receber:

message1
message2
message3

Na presença de múltiplos produtores concorrentes, a ordem global será determinada pela ordem efetiva de inserção na fila sob o mecanismo de sincronização.

Não é necessário garantir uma ordenação baseada em timestamps ou na ordem em que chamadas foram iniciadas em threads diferentes.

8. receive()
Assinatura
local message = mailbox:receive()

receive() é a operação bloqueante.

Se existir uma mensagem:

local message = mailbox:receive()

retorna imediatamente.

Se a fila estiver vazia, a unidade de execução deve ficar bloqueada até:

uma mensagem ser enviada; ou
a Mailbox ser fechada.

Não deve existir polling.

Não implementar:

while (queue.empty()) {
    // polling
}

Nem:

while (queue.empty()) {
    std::this_thread::yield();
}

Nem:

while (queue.empty()) {
    std::this_thread::sleep_for(...);
}

A implementação deve utilizar std::condition_variable.

9. condition_variable

A implementação interna deve utilizar o padrão clássico:

std::unique_lock<std::mutex> lock(mutex);


condition.wait(lock, [&] {
    return closed || !queue.empty();
});

A condition_variable permite que a thread seja colocada para dormir enquanto não houver trabalho.

Durante o wait():

mutex é liberado
       ↓
thread dorme
       ↓
outra thread altera o estado
       ↓
notify_one()
       ↓
thread acorda
       ↓
mutex é readquirido
       ↓
condição é verificada novamente

Isso evita consumo desnecessário de CPU.

10. Spurious Wakeup

A implementação deve considerar spurious wakeups.

Nunca assumir:

thread acordou
=
existe mensagem

Utilizar sempre um predicado:

condition.wait(lock, [&] {
    return closed || !queue.empty();
});

ou equivalente:

while (!closed && queue.empty()) {
    condition.wait(lock);
}

A primeira forma é preferível.

11. fetch()
Assinatura
local message = mailbox:fetch()

fetch() é a versão não bloqueante de receive().

Se houver mensagem:

local message = mailbox:fetch()

remove a próxima mensagem da fila e retorna a mensagem.

Se não houver mensagem:

nil

é retornado imediatamente.

Exemplo:

local message = mailbox:fetch()


if message ~= nil then
    process(message)
end
12. Diferença entre receive() e fetch()

A semântica deve ser extremamente clara:

mailbox:receive()

significa:

Aguarde até que exista uma mensagem.

Enquanto:

mailbox:fetch()

significa:

Tente obter uma mensagem agora. Se não houver, retorne imediatamente.

Tabela:

Operação	Bloqueia	Remove mensagem
send()	Não	Não
receive()	Sim, se vazia	Sim
fetch()	Não	Sim
isClosed()	Não	Não
close()	Não	Não
13. fetch() não deve utilizar condition_variable

fetch() nunca deve esperar.

O fluxo deve ser aproximadamente:

std::lock_guard<std::mutex> lock(mutex);


if (queue.empty()) {
    return nil;
}


auto message = queue.front();
queue.pop();


return message;

Naturalmente, a implementação real deve respeitar o modelo de ownership/conversão de valores do Arken.

14. close()
Assinatura
mailbox:close()

Fecha a Mailbox.

Depois:

mailbox:isClosed()

deve retornar:

true

close() deve ser idempotente.

Portanto:

mailbox:close()
mailbox:close()
mailbox:close()

não deve produzir comportamento indefinido.

15. close() deve acordar receive()

Esse é um dos requisitos mais importantes.

Imagine:

local message = mailbox:receive()

e a fila está vazia.

A thread está dormindo na condition_variable.

Se outra unidade executar:

mailbox:close()

o consumidor precisa ser acordado.

Conceitualmente:

Consumer
   │
   ▼
receive()
   │
   ▼
queue vazia
   │
   ▼
DORME
   │
   │
   │ close()
   ▼
ACORDA
   │
   ▼
closed == true
   │
   ▼
nil
16. Mensagens existentes durante close()

close() não deve descartar automaticamente as mensagens já enfileiradas.

Exemplo:

Antes de close():


queue:
    message1
    message2
    message3

Após:

mailbox:close()

o comportamento deve ser:

receive() → message1
receive() → message2
receive() → message3
receive() → nil

Ou seja:

close() impede novas mensagens, mas permite que as mensagens já existentes sejam drenadas.

Isso evita perda silenciosa de mensagens durante shutdown.

17. Estado da Mailbox

A Mailbox possui pelo menos:

OPEN
CLOSED

Internamente:

bool closed = false;

A condição de espera do consumidor é:

closed || !queue.empty()

Quando:

closed == false
queue.empty() == true

o consumidor dorme.

Quando:

queue.empty() == false

o consumidor pode consumir.

Quando:

closed == true
queue.empty() == true

receive() retorna nil.

18. send() após close()

Depois de:

mailbox:close()

não devem ser aceitas novas mensagens.

Portanto:

mailbox:send(message)

deve falhar de maneira determinística.

Não deve:

aceitar a mensagem;
reabrir a Mailbox;
recriar silenciosamente a Mailbox;
descartar silenciosamente a mensagem.

A implementação deve utilizar o mecanismo de erro padrão do Arken para informar que a Mailbox está fechada.

19. isClosed()
Assinatura
local closed = mailbox:isClosed()

Retorna:

true

ou:

false

A consulta deve ser thread-safe.

Entretanto, isClosed() não deve ser usado como mecanismo de sincronização.

Evitar:

if not mailbox:isClosed() then
    mailbox:send(message)
end

Isso possui uma race condition:

Thread A                 Thread B


isClosed() → false
                         close()
send()

O send() deve sempre validar internamente o estado.

20. Estrutura interna

A estrutura fundamental deve ser equivalente a:

struct MailboxState {
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<Message> queue;
    bool closed = false;
};

A implementação pode adaptar os tipos ao sistema interno do Arken.

O conceito, entretanto, deve permanecer:

MailboxState
├── mutex
├── condition_variable
├── FIFO queue
└── closed
21. Registry

Como a Mailbox é nomeada, deve existir um registry que associe:

nome → MailboxState

Conceitualmente:

std::unordered_map<
    std::string,
    std::shared_ptr<MailboxState>
>

O registry precisa ter sua própria sincronização.

Exemplo:

MailboxRegistry


"marketplace"
      │
      ▼
MailboxState #1


"orders"
      │
      ▼
MailboxState #2


"payments"
      │
      ▼
MailboxState #3
22. Não usar mutex global para as filas

O mutex utilizado pelo registry não deve ser utilizado para proteger as filas das Mailboxes.

Existem dois níveis distintos:

Registry mutex

Protege:

nome → MailboxState
Mailbox mutex

Protege:

queue
closed
estado da Mailbox

Isso permite que:

Mailbox A
Mailbox B
Mailbox C

sejam utilizadas simultaneamente sem todas competirem por um único mutex global.

23. Shared state

Objetos Lua diferentes que representam a mesma Mailbox devem compartilhar o mesmo estado interno.

Conceitualmente:

              Registry
                 │
                 ▼
          shared_ptr<State>
                 │
        ┌────────┴────────┐
        │                 │
     VM #1              VM #2
        │                 │
   Mailbox object     Mailbox object

A implementação pode utilizar:

std::shared_ptr<MailboxState>

ou o mecanismo equivalente já existente no Arken.

24. Garbage Collector

O garbage collector Lua não deve definir sozinho a semântica de fechamento.

A operação oficial é:

mailbox:close()

A destruição do objeto Lua deve apenas liberar a referência ao estado compartilhado.

Não assumir:

GC
=
close()

a menos que isso seja explicitamente necessário pela arquitetura do Arken.

25. Múltiplos produtores

A Mailbox deve suportar múltiplos produtores simultaneamente:

Producer A ──┐
Producer B ──┼──► Mailbox
Producer C ──┘

Todos podem chamar:

mailbox:send(message)

simultaneamente.

A fila deve permanecer consistente.

26. Múltiplos consumidores

A Mailbox deve suportar múltiplos consumidores:

                    ┌── Worker A
                    │
Mailbox ────────────┼── Worker B
                    │
                    └── Worker C

Uma mensagem deve ser entregue a apenas um consumidor.

A operação:

verificar fila
+
remover mensagem

deve ser atomicamente protegida pelo mutex.

Não pode haver possibilidade de dois consumidores retirarem a mesma mensagem.

27. notify_one()

Quando send() adicionar uma mensagem, deve preferencialmente executar:

condition.notify_one();

Não utilizar:

condition.notify_all();

como comportamento padrão.

Exemplo:

Mailbox


Worker 1 ─ waiting
Worker 2 ─ waiting
Worker 3 ─ waiting
Worker 4 ─ waiting


send(message)
      │
      ▼
notify_one()
      │
      ▼
um worker acorda

Isso evita o problema conhecido como thundering herd.

28. Locking de send()

A implementação pode seguir o padrão:

{
    std::lock_guard<std::mutex> lock(state->mutex);


    if (state->closed) {
        // erro
    }


    state->queue.push(message);
}


state->condition.notify_one();

A inserção e a verificação de closed precisam estar protegidas pelo mesmo mutex.

A notificação pode ocorrer depois da liberação do mutex.

29. Locking de receive()

Conceitualmente:

std::unique_lock<std::mutex> lock(state->mutex);


state->condition.wait(lock, [&] {
    return state->closed || !state->queue.empty();
});


if (state->queue.empty()) {
    // closed == true
    // não existem mais mensagens
    return nil;
}


auto message = state->queue.front();
state->queue.pop();


return message;

O unique_lock é necessário porque o condition_variable precisa liberar e readquirir o mutex durante o wait().

30. Locking de fetch()

Conceitualmente:

std::lock_guard<std::mutex> lock(state->mutex);


if (state->queue.empty()) {
    return nil;
}


auto message = state->queue.front();
state->queue.pop();


return message;

Nunca esperar na condition_variable.

31. Race condition importante: close vs send

A seguinte sequência precisa ser corretamente sincronizada:

Thread A                 Thread B


send(message)
                         close()

O estado final deve ser determinístico.

A operação que adquirir o mutex primeiro determina a ordem lógica.

Se send() entrar primeiro:

message é inserida
close acontece depois
message permanece na fila

Se close() entrar primeiro:

send() falha
message não é inserida

Nunca permitir:

close == true

e uma mensagem ser inserida posteriormente.

32. Race condition importante: close vs receive

Se houver mensagens:

queue:
    A
    B

e ocorrer:

close()

receive() deve continuar entregando:

A
B
nil

Não retornar nil imediatamente apenas porque closed == true.

A condição correta é:

closed && queue.empty()

para finalizar o consumidor.

33. Race condition importante: múltiplos consumers

Imagine:

queue:
    A

e:

Worker 1 → receive()
Worker 2 → receive()

Somente um deve receber A.

O mutex precisa proteger:

queue.empty()
queue.front()
queue.pop()

como uma operação indivisível em relação aos outros consumidores.

34. Queue

A implementação inicial deve utilizar uma estrutura FIFO apropriada, preferencialmente:

std::queue

ou uma estrutura equivalente já existente no Arken.

Não implementar uma lock-free queue na primeira versão sem uma necessidade comprovada.

A prioridade inicial é:

correção
+
segurança
+
simplicidade
+
previsibilidade

Depois podem ser avaliadas otimizações.

35. Mensagens

A Mailbox deve ser agnóstica ao conteúdo.

Não criar tipos específicos como:

MarketplaceMessage
OrderMessage
HttpMessage
TaskMessage

A camada de Mailbox deve transportar o mecanismo de valor/mensagem já utilizado pelo Arken.

A abstração deve pensar apenas em:

Message
36. Backpressure

A primeira versão não precisa implementar limite de capacidade.

Não implementar inicialmente:

Mailbox.open("marketplace", {
    capacity = 1000
})

A estrutura deve, entretanto, ser projetada de forma que uma capacidade máxima possa ser adicionada futuramente.

Importante:

send() não deve bloquear na primeira versão.

37. Não criar threads

A Mailbox não deve criar uma thread por Mailbox nem uma thread por mensagem.

Ela é uma estrutura de sincronização.

A thread/VM/worker que chama:

mailbox:receive()

é a unidade que fica bloqueada.

A Mailbox apenas fornece o mecanismo de espera e acordamento.

38. Não criar Event Loop

Não implementar:

mailbox:loop()

O Event Loop continua sendo responsabilidade do runtime.

A Mailbox deve fornecer somente:

send
receive
fetch
close
isClosed
39. Não utilizar callbacks

Não implementar inicialmente:

mailbox:onMessage(function(message)
    ...
end)

A API deve utilizar o modelo explícito:

local message = mailbox:receive()

Isso permite que o consumidor controle seu próprio fluxo.

Uma camada de maior nível poderia futuramente transformar uma Mailbox em callback/event handler, mas isso não faz parte da primitiva.

40. Exemplo: worker
local Mailbox = require("arken.concurrent.mailbox")


local mailbox = Mailbox.open("orders")


while true do
    local message = mailbox:receive()


    if message == nil then
        break
    end


    process(message)
end

Esse worker pode permanecer dormindo indefinidamente sem consumir CPU enquanto não houver mensagens.

41. Exemplo: produtor
local Mailbox = require("arken.concurrent.mailbox")


local mailbox = Mailbox.open("orders")


mailbox:send({
    action = "order.updated",
    order_id = 1234
})

O produtor retorna imediatamente após a mensagem ser colocada na fila.

42. Exemplo: fetch
local Mailbox = require("arken.concurrent.mailbox")


local mailbox = Mailbox.open("orders")


local message = mailbox:fetch()


if message ~= nil then
    process(message)
end

Se não existir mensagem:

fetch()
  ↓
nil

sem bloquear.

43. Exemplo de comunicação entre VMs

VM A:

local Mailbox = require("arken.concurrent.mailbox")


local mailbox = Mailbox.open("marketplace")


mailbox:send({
    action = "reconcile",
    marketplace_id = 1234
})

VM B:

local Mailbox = require("arken.concurrent.mailbox")


local mailbox = Mailbox.open("marketplace")


local message = mailbox:receive()


process(message)

A Mailbox não sabe nem precisa saber que existem duas VMs.

Para ela:

producer
   ↓
message
   ↓
mailbox
   ↓
consumer
44. Exemplo de shutdown

Worker:

local mailbox = Mailbox.open("orders")


while true do
    local message = mailbox:receive()


    if message == nil then
        break
    end


    process(message)
end

Shutdown:

mailbox:close()

Resultado:

worker acorda
    ↓
drena mensagens restantes
    ↓
receive() retorna nil
    ↓
worker termina
45. Semântica completa
Mailbox aberta + fila vazia
send()    → insere
receive() → bloqueia
fetch()   → nil
close()   → fecha
Mailbox aberta + fila com mensagens
send()    → insere
receive() → retorna próxima
fetch()   → retorna próxima
close()   → fecha, preservando fila
Mailbox fechada + fila com mensagens
send()    → erro
receive() → retorna próxima
fetch()   → retorna próxima
close()   → no-op
Mailbox fechada + fila vazia
send()    → erro
receive() → nil
fetch()   → nil
close()   → no-op
46. Tabela de estados
Estado	send	receive	fetch	close
aberta + vazia	insere	bloqueia	nil	fecha
aberta + mensagens	insere	mensagem	mensagem	fecha
fechada + mensagens	erro	mensagem	mensagem	no-op
fechada + vazia	erro	nil	nil	no-op
47. Desempenho esperado

O design deve ser eficiente sob alta concorrência.

O caminho normal de send() deve ser aproximadamente:

lock
push
unlock
notify_one
return

O caminho normal de fetch():

lock
check
pop
unlock
return

O caminho de receive() quando existe mensagem:

lock
check
pop
unlock
return

O caminho de receive() quando não existe mensagem:

lock
wait
    ↓
thread dorme
    ↓
notify_one
    ↓
acorda
check
pop ou close
unlock
return
48. Objetivos de concorrência

A implementação deve evitar:

busy waiting;
polling;
sleep artificial;
mutex global para todas as Mailboxes;
notify_all() desnecessário;
criação de threads por mensagem;
callbacks internos desnecessários;
race conditions;
deadlocks;
perda de mensagens durante shutdown;
duplicação de mensagens para consumidores.
49. Testes obrigatórios

A implementação deve possuir testes para pelo menos os seguintes cenários.

Teste 1 — send/receive
send(A)
receive() → A
Teste 2 — FIFO
send(A)
send(B)
send(C)


receive() → A
receive() → B
receive() → C
Teste 3 — receive bloqueante

Iniciar consumidor com:

receive()

sem mensagens.

Confirmar que ele permanece bloqueado.

Depois executar:

send(A)

e confirmar que o consumidor acorda e recebe A.

Teste 4 — fetch vazio
fetch() → nil

e confirmar que não bloqueia.

Teste 5 — fetch com mensagem
send(A)
fetch() → A
Teste 6 — múltiplos consumidores

Enviar N mensagens para M consumidores.

Confirmar:

quantidade total recebida == quantidade enviada

e:

nenhuma mensagem duplicada
Teste 7 — múltiplos produtores

Executar vários produtores simultaneamente.

Confirmar integridade da fila.

Teste 8 — close acordando receive

Consumidor bloqueado em:

receive()

executar:

close()

Confirmar que o consumidor acorda.

Teste 9 — drain após close

Enviar:

A
B
C

fechar.

Confirmar:

receive() → A
receive() → B
receive() → C
receive() → nil
Teste 10 — send após close

Confirmar que:

send(A)

após close() produz erro.

Teste 11 — close idempotente
close()
close()
close()

não deve produzir crash ou comportamento indefinido.

Teste 12 — concorrência close/send

Executar send() e close() simultaneamente em múltiplas threads e confirmar que não existe:

corrupção da fila;
mensagem aceita após fechamento;
deadlock;
crash;
comportamento indefinido.
50. Critérios de aceitação

A implementação somente deve ser considerada concluída quando:

Mailbox.open(name) retorna uma referência para a Mailbox nomeada.
Duas unidades de execução podem abrir o mesmo nome e compartilhar o estado.
send() é não bloqueante em relação ao consumidor.
send() é thread-safe.
receive() bloqueia somente quando necessário.
receive() utiliza std::condition_variable.
receive() trata corretamente spurious wakeups.
fetch() nunca bloqueia.
fetch() remove a mensagem da fila.
A fila é FIFO.
Múltiplos produtores são suportados.
Múltiplos consumidores são suportados.
Cada mensagem é consumida uma única vez.
close() é thread-safe.
close() é idempotente.
close() acorda consumidores bloqueados.
Mensagens existentes podem ser drenadas após close().
receive() retorna nil quando a Mailbox está fechada e não possui mensagens.
fetch() retorna nil quando não existem mensagens.
send() falha quando a Mailbox está fechada.
Não existe polling.
Não existe busy waiting.
Não existe um mutex global único protegendo todas as filas.
A Mailbox não cria threads próprias.
A Mailbox não possui event loop próprio.
A Mailbox não depende de callbacks.
A API permanece restrita aos métodos definidos neste documento.
51. API final

A API oficial da primeira versão é exatamente:

local Mailbox = require("arken.concurrent.mailbox")


local mailbox = Mailbox.open("marketplace")


mailbox:send(message)


local message = mailbox:receive()


local message = mailbox:fetch()


local closed = mailbox:isClosed()


mailbox:close()

Semântica resumida:

open()
    abre/acessa uma Mailbox nomeada


send()
    adiciona mensagem
    não bloqueia


receive()
    aguarda mensagem
    remove e retorna mensagem
    retorna nil quando fechada e sem mensagens


fetch()
    tenta obter mensagem
    não bloqueia
    retorna nil se não houver mensagem


isClosed()
    consulta o estado


close()
    fecha
    impede novos sends
    acorda receivers bloqueados
    permite drenar mensagens já enfileiradas
52. Resumo arquitetural

A Mailbox é uma primitiva baseada em:

                Mailbox
                   │
                   ├── Name
                   │
                   └── MailboxState
                          │
                          ├── mutex
                          ├── condition_variable
                          ├── FIFO queue
                          └── closed

O registry fornece:

name → shared MailboxState

O modelo de execução é:

             PRODUCERS
                 │
                 │ send()
                 ▼
        ┌─────────────────┐
        │     Mailbox     │
        │                 │
        │   FIFO Queue    │
        │                 │
        └────────┬────────┘
                 │
          ┌──────┴──────┐
          │             │
     receive()       fetch()
       bloqueia      não bloqueia
          │             │
          ▼             ▼
       CONSUMERS / WORKERS

A responsabilidade da Mailbox termina em:

armazenar, sincronizar e entregar mensagens.

Ela não deve ser responsável por:

executar tarefas
gerenciar threads
gerenciar VMs
gerenciar event loops
gerenciar HTTP
gerenciar WebSockets
processar mensagens
implementar callbacks

Isso mantém a abstração pequena, generalista e adequada ao modelo MVM do Arken, sem acoplar a biblioteca ao motivo pelo qual a mensagem está sendo enviada.



