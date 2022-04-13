#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define QTD_THREADS 5
//int sleep_time;

void* printHello(void* p){
    long index = (long)p;
    printf("Olá sou a thread #%ld\n", index);
    sleep((int)index+1);
    //sleep(sleep_time;
    printf("[%ld] dormi %d segundos, vou sair...\n", index, sleep_time);
    //sleep_time--;
    pthread_exit((void*)index);
} 

int main(void) {
    pthread_t threads[QTD_THREADS];
    int ret_vals[QTD_THREADS];
    //sleep_time = 5;
    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, printHello, (void*)i);
    }

    for(int i = 0; i < QTD_THREADS; i++){
        pthread_join(threads[i], (void **)&ret_vals[i]);
        printf("thread #%d retornou o valor: %d\n", i, ret_vals[i]);
    }
    return 0;
}