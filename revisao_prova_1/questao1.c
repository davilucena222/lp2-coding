#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD_PROCESSOS 30
#define QTD_THREADS 30
#define SLEEP_TIME 2

//função para a thread executar como parâmetro
void* funcao_thread(void* p){
    //cada thread vai dormir por 2 segundos
    sleep(SLEEP_TIME);
}

int main(void) {
    //variáveis para medir o tempo de criação do processo
    clock_t tempo_processo1, tempo_processo2, tempo_resultante_processo;

    //variáveis para medir o tempo de criação das threads
    clock_t tempo_thread1, tempo_thread2, tempo_resultante_thread;

    //variável do tipo array para criar 30 threads
    pthread_t threads[QTD_THREADS];

    //variável para pegar o pid do processo original
    int pid_original;

    //variável para alterar o contexto dos processos no tempo de execução
    int new_p;

    //capturando o pid do processo original
    pid_original = getpid();

    //iniciando a medição de tempo antes de criar 30 processos
    tempo_processo1 = clock();

    //bloco for para criar 30 processos
    for(int i = 0; i < QTD_PROCESSOS; i++){
        //verificando se é o processo original 
        if(pid_original == getpid()){
            new_p = fork();
        }
    }

    //colocando todos os processos clonados para dormirem 2 segundos
    if(pid_original != getpid()){
        sleep(2);
        exit(0);
    }

    //entrando nos processos originais criados para calcular o tempo de criação
    if(pid_original == getpid()){
        //finalizando a medição de tempo depois de criar 30 processos
        tempo_processo2 = clock(); 

        //calculando o tempo total que levou para criar os 30 processos
        tempo_resultante_processo = tempo_processo2 - tempo_processo1;

        printf("Intanciar 30 processos!\n");
        printf("Tempo de execução e criação de 30 processos: %lf ms\n", ((double)tempo_resultante_processo)/((CLOCKS_PER_SEC/1000)));
        //fim da medição do tempo de criação e execução dos 30 processos (apenas no processo original)

        /**************** Criação das 30 threads *****************/
        //iniciando a medição de tempo antes de criar 30 threads
        tempo_thread1 = clock();

        //bloco for para realizar a criação das 30 threads
        for(int i = 0; i < QTD_THREADS; i++){
            pthread_create(&threads[i], NULL, funcao_thread, NULL);
        }

        //finalizando a medição de tempo depois de criar 30 threads
        tempo_thread2 = clock();

        //calculando o tempo total que levou para criar as 30 threads
        tempo_resultante_thread = tempo_thread2 - tempo_thread1;
        printf("Instanciando 30 threads!\n");
        printf("Tempo de execução e criação de 30 threads: %lf ms\n", ((double)tempo_resultante_thread)/((CLOCKS_PER_SEC/1000)));
        //fim da medição do tempo de criação e execução das 30 threads (apenas no processo original)
    }

    return(0);
}

//De acordo com a execução do programa, o tempo de criação de 30 threads se mostra mais eficaz em relação aos processos. Processos levam mais tempo para serem criados, enquanto as threads possuem menos recursos disponíveis, o que impacta diretamente no tempo de criação e execução que é menor em comparação aos processos.

//Processos ao serem clonados possuem um maior contexto em sua composição, tudo o que está no processo original será copiado/repassado para o processo clonado, o contexto de cada processo é independente, ou seja, se uma alteração é feita no processo original ela não vai afetar o processo clonado.
//Dentro de um processo existe pelo menos uma thread, por exemplo, dentro do processo original de um programa escrito em C a função main é uma thread desse programa. 

//Enquanto que as threads possuem um contexto menor, seu tempo de criação é bem menor em comparação aos processos e são bem mais leves quando se trata de execução, no entanto as threads compartilham de variáveis globais e contextos globais, ou seja, se uma variável global é alterada/atualizada por uma thread todas as outras threads precisam saber que alteração foi essa para não confundir as outras ao executar tal variável ou contexto global em um outro momento.