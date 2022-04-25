    #Sincronismo condicional:
        funções wait() no caso de processos - para esperar os processos serem executados e não terminar o processo principal.
        função pthread_join() no caso de threads - para esperar as threads realizarem a devida execução e manter a thread principal viva até que a execução finalize.

    #Seção Crítica:
        É precedida por um protocolo de entrada seguida por um protocolo de saída que assume um escopo de execução específico.
        É um bloco de código que é importante para o programe e que precisa de cuidado para ser executado de forma correta.

    #Seção não-crítica:
        É a parte do código que não precisa de travas.

    #Locks (travas):
        Protege um bloco de código (recurso compartilhado - variável global) de ser executado de forma concorrente (ao mesmo tempo) para não causar danos ou perder dados. Permite o bloco de código ser executado 1 vez por execução com monitoramento da trava.

    #Problemas da seção crítica:
        Para acessar uma seção crítica é preciso satisfazer algumas propriedades.

        #Exclusão mútua: no máximo um processo pode estar executando a sua seção crítica em um determinado instante. Não pode existir   mais de um processo executando a seção crítica.

        #Ausência de Deadlock(Livelock): Se dois ou mais processos estão tentando entrar em suas seções críticas, no mínimo um (processo apenas) obterá sucesso. É preciso ter uma ordem para definir qual dos processos devem executar primeiro a execução crítica. Exemplo: "API thread_safe" é uma forma de estabelecer que as threads antes de executarem a seção crítica devem atender a alguns protocolos.

        #Ausência de atraso desnessário: Se um processo está tentando entrar em sua seção crítica e os outros processos estão executando seções não-crítica (ou já terminaram), o primeiro não pode ser impedido de continuar.

        #Entrada eventual: Um processo que está tentando entrar em sua seção crítica, eventualmente, obterá sucesso.
        Starvation - matar uma thread que não executou a seção crítica.

    #Resolvendo problemas de seção crítica (sintaxe de Andrews):
        #Uma forma trivial de resolver o problema da seção crítica seria envolvê-la em parênteses angulares <SC> para que seja executada em forma atômica (uma parte da execução fica parada até que a manipulação com algo específico seja finalizado, em seguida é liberada para a próxima execução - não é para dividir a execução).
        #Comando <await> pode ser solução.

        #Políticas de escalonamento:
            - a maioria das propriedades "liveness" precisam de um escalonamento justo para serem satisfeitas;
            - uma política de escalonamento determina qual processo será o próximo a ser executado.