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

int var_global = 0;
int number = 1;
int next = 1;
int turn[QTD_THREADS];

void* cs(void* p){
    long idx = (long)p;
    
    printf("[%ld] iniciou\n", idx);
    
    while(1){
        //<turn[i] = number; number++;>
        //protocolo de entrada
        //garantindo que todas as threads recebem um ticket diferente (acesso a seção crítica 1 por vez)
        turn[idx] = __sync_fetch_and_add(&number, 1); //utilizando uma função para incrementar a trava
        //<await(turn[i] == next);>
        while(!(turn[idx] == next)) usleep(100); //enquanto não for a vez dele fica bloqueado
        //seção crítica
        var_global = var_global + 1;
        printf("[%ld] variável global: %d\n", idx, var_global);
        puts("..");
        //<next++;>
        next = next + 1; //protocolo de saída (destrava o código)
    }
}

int main(void){
    pthread_t threads[QTD_THREADS];

    for(int i = 0; i < QTD_THREADS; i++){
        turn[i] = 0;
    }


    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, cs, (void*)i);
    }

    sleep(10);

    return(0);
}