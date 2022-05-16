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

#define QTD_GENERATE 10

int buf;
sem_t empty, full;

void* prod(void* p){
    int valor_prod;
    int count = 0;

    while(count < QTD_GENERATE){
        sem_wait(&empty);
        valor_prod = rand() % 255;
        printf("[PROD] %d\n", valor_prod);
        sleep(2);
        buf = valor_prod;
        sem_post(&full);
        count++;
    }
}

void* cons(void* p){
    int valor_cons;
    int count = 0;

    buf = 0;

    while(count < QTD_GENERATE){
        sem_wait(&full);
        valor_cons = buf;
        printf("[CONS] %d\n", valor_cons);
        sleep(1);
        sem_post(&empty);
        count++;
    }
}


int main(void){
    pthread_t _prod, _cons;
    time_t t;

    srand(time(&t));

    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);

    pthread_create(&_prod, NULL, prod, NULL);
    pthread_create(&_cons, NULL, cons, NULL);

    pthread_join(_prod, NULL);
    pthread_join(_cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);


    return(0);
}