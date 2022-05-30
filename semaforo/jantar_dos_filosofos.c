#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Jantar dos Filósofos */

#define QTD_THREADS 5

int running = 1;
int eat_count[QTD_THREADS];
int think_count[QTD_THREADS];

pthread_t phi[QTD_THREADS];
sem_t garfos[QTD_THREADS];

int eat(long i) {
    printf("[%ld] Comendo... (%d)\n", i, eat_count[i]++);
    sleep((int)i);
}

int think(long i) {
    printf("[%ld] Pensando... (%d)\n", i, think_count[i]++);
    sleep((int)i);
}

void* filosofo(void* p) {
    long idx = (long)p;
    if (idx == (QTD_THREADS - 1)) {
        while (running) {
            sem_wait(&garfos[0]);
            sem_wait(&garfos[QTD_THREADS - 1]);
            eat(idx);
            sem_post(&garfos[0]);
            sem_post(&garfos[QTD_THREADS - 1]);
            think(idx);
        }
    } else {
        while (running) {
            sem_wait(&garfos[idx]);
            sem_wait(&garfos[idx + 1]);
            eat(idx);
            sem_post(&garfos[idx]);
            sem_post(&garfos[idx+1]);
            think(idx);
        }
    }
}

int main(void) {

    for (int i = 0; i < QTD_THREADS; i++) {
        sem_init(&garfos[i], 0, 1);
        eat_count[i] = 0;
        think_count[i] = 0;
    }

    for (long i = 0; i < QTD_THREADS; i++) {
        pthread_create(&phi[i], 0, filosofo, (void*)i);
    }

    sleep(30);
    running = 0;

    for (int i = 0; i < QTD_THREADS; i++) {
        pthread_join(phi[i], 0);
    }

    for (int i = 0; i < QTD_THREADS; i++) {
        sem_destroy(&garfos[i]);
    }

    return 0;

}
