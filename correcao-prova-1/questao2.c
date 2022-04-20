#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define LIMITE_VAR 100
#define QTD_THREADS 10

//função para proteger uma linha ou bloco de código
//trava e destrava a seção crítica de um código ou comando
pthread_mutex_t mutex_var_global = PTHREAD_MUTEX_INITIALIZER;

double var_global;

int gera_aleatorio(int teto) {
    //return rand() % teto;
    return 99;
}

void* altera_global(void* p){
    long index = (long)p;

    printf("Sou a thread %ld e vou alterar a variável global\n", index);

    if((index % 2 == 0)){
        for(int i = 0; i < 50; i++){
            //bloqueando a thread para realizar uma execução por vez
            pthread_mutex_lock(&mutex_var_global);
            var_global = var_global + 1;
            //desbloqueando a thread para realizar a próxima execução
            pthread_mutex_unlock(&mutex_var_global);
            printf("[%ld] var_global: %.2lf\n", index, var_global);
        }
        int sleep_time = (rand() % 4) + 1;
        //sleep(sleep_time);
    } else {
        for(int i = 0; i < 30; i++){
            //bloqueando a thread para realizar uma execução por vez
            pthread_mutex_lock(&mutex_var_global);
            var_global = var_global - 1;
            //desbloqueando a thread para realizar a próxima execução
            pthread_mutex_unlock(&mutex_var_global);
            printf("[%ld] var_global: %.2lf\n", index, var_global);
        }
        //sleep(2);
    }
}

int main(void){
    int fork_return;
    int child_return;
    time_t t;
    pthread_t threads[QTD_THREADS];

    srand(time(&t));

    fork_return = fork();
    printf("(main) - var_global: %2.lf\n", var_global);
    if(fork_return == 0){
        //processo clonado
        return gera_aleatorio(LIMITE_VAR);
    } 
    
    wait(&child_return);
    var_global = WEXITSTATUS(child_return);
    printf("(main) - var_global: %2.lf\n", var_global);

    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, altera_global, (void*)i);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("(main) - var_global: %2.lf\n", var_global);


    return(0);
}