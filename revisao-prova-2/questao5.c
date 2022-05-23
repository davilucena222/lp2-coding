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

#define QTD_GENERATE 100
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int frente = 0;
int tras = 0;
sem_t vazio;
sem_t cheio;

void* threadProdutora(void* p){
  int contador = 0;
  int valor;

  while(contador < QTD_GENERATE){
    sem_wait(&vazio);//decrementa de 1 para 0 (sinalizando que a seção crítica está em uso)
    valor = rand() % 255;
    printf("[ESCOPO PRODUTOR] - Valor = %d\n", valor);
    sleep(1);
    buffer[tras] = valor;
    tras = (tras + 1) % BUFFER_SIZE;
    sem_post(&cheio);//incrementa de 0 para 1 (sinalizando que a seção crítica está livre)
    contador += 1;
  }
}

void* threadConsumidora(void* p){
  int contador = 0;
  int valor_consumido;

  for(int i = 0; i < BUFFER_SIZE; i++){
    buffer[i] = 0;
  }

  while(contador < QTD_GENERATE){
    sem_wait(&cheio);//decrementa de 1 para 0 (sinalizando que a seção crítica está em uso)
    valor_consumido = buffer[frente];
    frente = (frente + 1) % BUFFER_SIZE;
    printf("[ESCOPO CONSUMIDOR] - Valor = %d\n", valor_consumido);
    sleep(2);
    sem_post(&vazio);//incrementa de 0 para 1 (sinalizando que a seção crítica está livre)
    contador += 1;
  }
}

int main(void){
  pthread_t produtora;
  pthread_t consumidora;

  // sem_init(&vazio, 0 , BUFFER_SIZE);
  sem_init(&vazio, 0 , 1);
  sem_init(&cheio, 0 , 0);

  time_t tempo;
  srand(time(&tempo));

  pthread_create(&produtora, NULL, threadProdutora, NULL);
  pthread_create(&consumidora, NULL, threadConsumidora, NULL);

  pthread_join(produtora, NULL);
  pthread_join(consumidora, NULL);

  sem_destroy(&vazio);
  sem_destroy(&cheio);

  return(0);
}