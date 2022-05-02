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

//variáveis globais compartilhadas que não são sobrescritas na mesma execução, pois todas as interações e escritas estão sendo feitas por funções atômicas (ou seja, uma execução por vez, uma alteração por vez)
//as variáveis globais também são alteradas de forma distinta a cada execução devido o uso de funções atômicas
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
        //compara o ticket com o valor de next que foi incrementado na execução anterior
        //! - enquanto não for diferente executa e prossegue em direção à seção crítica
        //alocando um único valor para uma única thread evita a contenção de memória 
        while(!(turn[idx] == next)) usleep(50); //enquanto não for a vez dele fica bloqueado
        //seção crítica
        var_global = var_global + 1;
        printf("[%ld] variável global: %d\n (ticket: %d)\n", idx, var_global, turn[idx]);
        puts(".."); 
        //<next++;>
        __sync_fetch_and_add(&next, 1); //variável que é incrementada de forma atômica e utilizada dentro do escopo do while para verificar se o valor do ticket (que também é atômico) é igual a ela (next), isso evita que mais de uma thread entre na seção crítica
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

    sleep(20);

    return(0);
}