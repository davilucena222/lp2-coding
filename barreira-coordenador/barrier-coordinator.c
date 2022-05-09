#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>

#define QTD_THREADS 5
#define LIMITE_CONT_JOBS 10

//thread barreira 
pthread_barrier_t barreira;

int contador_cont_jobs = 0;

//função que é executada 5 vezes em um único conjunto que possui tamanho 10
void* worker(void* p){
    while(contador_cont_jobs < LIMITE_CONT_JOBS){
        long idx = (long)p;

        int sleep_time = rand() % 8;
        printf("[%ld] - job: %d \n", idx, sleep_time);
        sleep(sleep_time);

        //sleep(10 + idx);
        printf("[%ld] terminei o job!\n", idx);
        //ponto de embarreiramento - fica bloqueado até a última thread chegar e ela é a primeira a ser liberada da barreira após todas chegarem
        pthread_barrier_wait(&barreira); 
        printf("[%ld] Passei da barreira!\n", idx);
    }
}

int main(void){
    //criaindo threads
    pthread_t threads[QTD_THREADS];

    //inicializando a thread barreira para bloquear a quantidade exata de threads que vai executar a função worker
    //thread que estão sendo criadas e a thread main (para incrementar o valor do contador)
    pthread_barrier_init(&barreira, NULL, QTD_THREADS + 1);

    time_t tempo;
    srand(tempo);

    //criando as threads
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    //bloco while para executar cada conjunto até 10 vezes
    while(contador_cont_jobs < LIMITE_CONT_JOBS){
        printf("[main] conjunto: %d\n", contador_cont_jobs);
         //esperando cada conjunto (com 5 execuções) terminar 
        pthread_barrier_wait(&barreira);
        __sync_fetch_and_add(&contador_cont_jobs, 1); //incrementando a variável contador de forma atômica
    }

    //puxando a thread de volta para a execução principal
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    //destruindo a barreira
    pthread_barrier_destroy(&barreira);


    return(0);
}