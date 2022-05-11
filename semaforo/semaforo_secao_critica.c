
//RESOLUÇÃO PARA SEÇÃO CRÍTICA
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <semaphore.h>

#define QRD_THREADS 5

//cdeclarando um semáforo
sem_t mutex;
int var_global = 0;

void* worker(void* p){
    long idx = (long)p;
    //semátoro para bloquear a thread seguinte (decrementa para zero)
    //as threads seguintes não conseguem entrar na seção crítica devido já ter uma dentro
    //a próxima thread só consegue entrar após a anterior terminar sua tarefa dentro da seção crítica
    sem_wait(&mutex);
    printf("[%ld] - dentro da seção crítica!\n", idx);
    var_global += idx;
    sleep(1);
    //semátoro para liberar a thread (incrementa para um), sinalizar que saiu da seção crítica
    sem_post(&mutex);
    printf("[%ld] - fora da seção crítica: %d!\n", idx, var_global);
}

int main(void){
    //pthread_t w1, w2;
    pthread_t w[QRD_THREADS];

    //inicializando o semáforo com o valor 1, o zero é apenas uma flag
    sem_init(&mutex ,0, 1);

    for(long i = 0; i < QRD_THREADS; i++){
        pthread_create(&w[i], 0, worker, (void*)i);
    }

    for(int i = 0; i < QRD_THREADS; i++){
        pthread_join(w[i], 0);
    }

    //destruindo o semáforo
    sem_destroy(&mutex);

    /*pthread_create(&w1, 0, worker1, 0);
    pthread_create(&w2, 0, worker2, 0);

    pthread_join(w1, 0);
    pthread_join(w2, 0);*/


    return(0);
}