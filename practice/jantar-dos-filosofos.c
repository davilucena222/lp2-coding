#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define QTD_THREADS 5
int running = 1;

//thread para comer o macarrão
int eat_count[QTD_THREADS];

//threads para pensarem 
int think_count[QTD_THREADS];

//criando threads
pthread_t phi[QTD_THREADS];

//criando 5 semáforos (1 para cada thread) para delimitar quais filósofos podem pegar os garfos
sem_t garfos[QTD_THREADS];

//função que será executada pela thread que estará comendo o macarrão
int eat(long i){
    printf("[%ld] - Comendo... (%d)\n", i, eat_count[i]++);
    sleep((int)i);
}

//função que será executada pela thread que estará pensando
int think(long i){
    printf("[%ld] - Pensando... (%d)\n", i, think_count[i]);
    sleep((int)i);
}

//função que realizará a dinâmica
void* filosofo(void* p){
    long idx = (long)p;

    //verificando se o idx é igual a 4 (ou seja a thread da posição 3) para que a última thread (posição 4) não pegue o garfo 
    if(idx == (QTD_THREADS - 1)){
        while(running){
            //bloqueando as threads 
            sem_wait(&garfos[0]);
            sem_wait(&garfos[QTD_THREADS - 1]);//5 - 1 = 4 (sem_wait() na penúltima thread posição 3)
            eat(idx);
            //liberando as threads
            sem_post(&garfos[0]);
            sem_post(&garfos[QTD_THREADS - 1]);
            think(idx);
        }
    } else {
        while(running){
            //bloqueando threads
            sem_wait(&garfos[idx]);
            sem_wait(&garfos[QTD_THREADS - 1]);
            eat(idx);
            //liberando threads
            sem_post(&garfos[idx]);
            sem_post(&garfos[QTD_THREADS - 1]);
            think(idx); 
        }
    }
}

int main(void){
    //atribuindo zero para as threads
    for(int i = 0; i < QTD_THREADS; i++){
        //criando semáforos
        sem_init(&garfos[i], 0, 1);
        eat_count[i] = 0;
        think_count[i] = 0;
    }

    //criando as threads que vão executar 
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&phi[i], 0, filosofo, (void*)i);
    }

    //sleep na execução principal
    sleep(30);

    //após as criações das threads atribui zero para a variável running
    running = 0;

    //trazendo de volta as execuções de volta para a execução principal
    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(phi[i], 0);
    }

    //destruindo os semáforos
    for(int i = 0; i < QTD_THREADS; i++){
        sem_destroy(&garfos[i]);
    }


    return(0);
}