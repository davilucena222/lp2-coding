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
int count = 0;

void* worker(void* p){
    while(1){
        long idx = (long)p;

        int sleep_time = idx + 1;
        printf("[%ld] task = sleep %d : count = %d\n", idx, sleep_time, count);
        puts("..");
        sleep(sleep_time); //task idx

        printf("[%ld] task done\n", idx);
        puts("..");

        __sync_fetch_and_add(&count, 1);
        while((count % QTD_THREADS) != 0) usleep(50);
        printf("%ld saindo da barreira...\n", idx);
        puts("..");
    }
}

int main(void) {
    pthread_t threads[QTD_THREADS];

    for(long i = 0; i < QTD_THREADS; i++){
        pthread_create(&threads[i], NULL, worker, (void*)i);
    }

    sleep(10);




    return(0);
}