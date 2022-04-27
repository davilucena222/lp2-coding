#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>

int lock = 0;
int var_global = 0;

void* cs(void* p){
    long idx = (long)p;
    while(1){
        //testa e "seta" a variável para 1 no mesmo ciclo
        //lock = 0 sai do while e o valor de lock passa a ser 1
        //__atomic_test_and_set() - altera o valor de lock
        while(__atomic_test_and_set(&lock, 1)); //protocolo de entrada
        var_global++;
        printf("sessão crítica %ld variável global: %d\n", idx, var_global);
        puts("..");
        //sinaliza para a outra thread que ela pode executar o bloco
        lock = 0; //protocolo de saída

        //mesmo com o sleep() a thread pode voltar e executar o bloco de novo, o que não deveria acontecer
        usleep(50);
        printf("sessão crítica %ld variável global (segundo printf): %d\n", idx, var_global);
    }
}

int main(void){
    pthread_t threads[2];

    pthread_create(&threads[0], NULL, cs, (void*)1);
    pthread_create(&threads[1], NULL, cs, (void*)2);
    sleep(10);

    return(0);
}