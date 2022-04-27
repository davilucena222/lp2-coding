#História de um estado de execução de processo/programa: sequência de estados que ele passa por um determinado instante. Cada execução de linhas é um estado diferente.

#Cada linha convertida em código C vira instruções para linguagem de máquina (assembly)
    #Cada instrução terá "tics" (1 núcleo de processamento -> clocks) diferentes, e variáveis podem ser alocadas para outros espaços de memória. 
    #Além disso, pode haver escalonamento, o que não é algo vantajoso.

#Estado de execução de um processo: valores disponíveis na região de memória reservada. É o estado de uma memória em um determinado momento da execução. São realizadas instruções para realizar a mudança de estados e dentro dessas instruções existem estados intermediários que realizam incrementos e cópias de memória. Tais valores terminam em diferentes espaços de memória, ao alterar uma variável sua posição na memória é diferente.

#Estabelecer uma sessão crítica no código é também "avisar" as outras threads que ela está executando ou modificando alguma variável. É deixar elas cientes de que existem estados (estados intermediários) armazenando as alterações de tal variável. Tudo isso deve ser estabelecido de forma atômica (para "acionar" as demais threads que não é viável ter acesso a sessão crítica naquele determinado momento onde está ocorrendo mudanças, incrementos, cópias e estados intermediários dentro dos estados de execução), o escalonador escalona incrementos, alterações e instruções, mas deve ser garantido no código que outras threads não devem ter acesso aos estados intermediários (alterações específicas dentro do escopo da sessão crítica). 

#Contenção de memória: a grande quantidade de leitura e escrita em loops envolvendo instruções TEST AND SET podem causar a contenção de memória e gerar um overhead (consumo extra) adicional com a invalidação de cache. O processo precisa consultar a memória cache e verificar se as instruções, variáveis, dados estão atualizados para os outros processadores e isso é desperdício de processamento, pois o processador é quem realiza essa atividade, gasta a energia dele.

#Parênteses angulares -> criar uma sessão crítica, protocolo de entrada e de saída.

#Forma atômica: garante que a thread não vai acessar escopos que não devem ser compartilhados (variáveis, incrementos, blocos de memória), ou seja, indivisíveis.