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

#define QTD_LETRAS 6

pthread_barrier_t barreira;
int contador_cont_jobs = 0;
int palavra[QTD_LETRAS];

void* geraLetra(void* p){
    long idx = (long)p;

    while(1){
        char c = 65 + rand() % 23;

        palavra[idx] = c;
        pthread_barreier_wait(&barreira);
    }
    
}

int main(void){




    return(0);
}