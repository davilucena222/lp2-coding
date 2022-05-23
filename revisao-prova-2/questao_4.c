#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

#define QTD_LETRAS 6
#define QTD_EXEC 10

char palavra[QTD_LETRAS];
int palavras_geradas = 0;

pthread_barrier_t barreira;

void* gera_letra(void* p) {
    long id = (long)p;
    while (palavras_geradas < QTD_EXEC) {
        char letra = 65 + (rand() % 26);
        palavra[id] = letra;
        pthread_barrier_wait(&barreira);
    }
}

int main(void) {
    pthread_t threads[QTD_LETRAS];
    time_t t;

    srand(time(&t));

    pthread_barrier_init(&barreira, NULL, QTD_LETRAS + 1);

    for (long x = 0; x < QTD_LETRAS; x++){
        pthread_create(&threads[x], NULL, gera_letra, (void*)x);
    }


    while (palavras_geradas < QTD_EXEC) {
        pthread_barrier_wait(&barreira);

        int x = 0;
        int letras_iguais = 0;

        do {
            if (palavra[x] == palavra[QTD_LETRAS - (x + 1)]) {
                letras_iguais++;
            }
            x++;
        } while (x != QTD_LETRAS);

        printf("palavra = %s\n", palavra);

        if (letras_iguais == QTD_LETRAS) {
            printf("Palindromo\n\n");
        } else {
            printf("Nao eh palindromo\n\n");
        }
        __sync_fetch_and_add(&palavras_geradas, 1);
    }

    // Barreira da main, pois as threads ainda estão em execução
    pthread_barrier_wait(&barreira);
    pthread_barrier_destroy(&barreira);

    return 0;
}
