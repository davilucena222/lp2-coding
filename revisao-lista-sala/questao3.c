/* Seção Crítica em um sistema bancário múltiplas podem realizar operações de débito e crédito */
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

#define QTD_THREADS 10

float saldoCliente = 0;
pthread_mutex_t mutexSaldo;

void* operacao(void* p){
    long valor_op = (long)p;

    pthread_mutex_lock(&mutexSaldo);
    saldoCliente += valor_op;
    pthread_mutex_unlock(&mutexSaldo);
}

int main(void){
    pthread_t threads[10];

    






    return(0);
}