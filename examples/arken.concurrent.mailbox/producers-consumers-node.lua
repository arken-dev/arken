-- Nó do exemplo producers-consumers.lua. O mesmo arquivo serve tanto de
-- produtor quanto de consumidor, dependendo de params.role — só pra evitar
-- duplicar o boilerplate de abrir a mailbox em dois arquivos separados.

local Mailbox = require('arken.concurrent.mailbox')

return function(task, params)
  local mailbox = Mailbox.open('work-queue')

  if params.role == 'producer' then

    for i = 1, params.count do
      mailbox:send('job from producer ' .. params.id .. ' #' .. i)
    end

  else -- consumer

    -- cada mensagem é entregue a exatamente um consumidor: o mutex interno
    -- da Mailbox garante que "checar fila + remover mensagem" é atômico,
    -- então nenhum job é processado duas vezes nem perdido entre os
    -- consumidores concorrentes.
    while true do
      local job = mailbox:receive()
      if job == nil then
        break
      end
      print('[consumer ' .. params.id .. '] ' .. job)
      -- simula processamento, só pra dar chance dos outros consumidores
      -- pegarem trabalho também neste exemplo (sem isso, um consumidor
      -- rápido o bastante poderia drenar a fila inteira sozinho antes dos
      -- demais serem escalonados — ainda correto, só menos ilustrativo)
      os.sleep(0.05)
    end

  end
end
