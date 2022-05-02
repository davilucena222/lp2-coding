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

int arrive[QTD_THREADS];
int proceed[QTD_THREADS]; 

void* worker(void* p){
    long idx = (long)p;
    int sleep_time = idx + 1;

    while(1){
        //printf("[%ld] sleep = %d\n", idx, sleep_time);
        puts("..");
        sleep(sleep_time);
        printf("[%ld] task done...\n", idx);
        puts("..");
        arrive[(int)idx] = 1;
        while(proceed[(int)idx] != 1) usleep(50); //bloqueia a thread
        proceed[(int)idx] = 0;
        printf("[%ld] saindo da barreira...\n", idx);
    }
}

//função que preenche e altera os valores do array
void* coordinator(void* p){
    while(1) {
        for(int i = 0; i < QTD_THREADS; i++){
            while(arrive[i] != 1) usleep(50);
            arrive[i] = 0;
        }
        for(int i = 0; i < QTD_THREADS; i++){
            proceed[i] = 1;
        }
    }
}

int main(void){
    pthread_t threads[QTD_THREADS];
    pthread_t t_coord;

    //zerando as posições dos dois arrays
    for(int i = 0; i < QTD_THREADS; i++){
        arrive[i] = 0;
        proceed[i] = 0;
    }

    pthread_create(&t_coord, NULL, coordinator, NULL);

    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    sleep(20);

    return(0);
}