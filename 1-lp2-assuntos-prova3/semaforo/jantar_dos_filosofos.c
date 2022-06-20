#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//definindo a quantidade de threads
#define QUANTIDADE_THREADS 5

//variável para o loop infinito while()
int running = 1;

//threads para comerem o macarrão
int eat_count[QUANTIDADE_THREADS];

//threads para pensarem após enquanto outros filósofos comem o macarrão
int think_count[QUANTIDADE_THREADS];

//referenciando as threads de filósofos
pthread_t phi[QUANTIDADE_THREADS];

//semáforos para manipular o acesso aos garfos
sem_t garfos[QUANTIDADE_THREADS];

//função para simular os filósofos comendo
int eat(long i){
  printf("[%ld] - Comendo... (%d)\n", i, eat_count[i]++);//incrementando a quantidade de quem já comeu
  sleep((int)i);//tempo do sleep de acordo com o índice da thread
}

//função para simular o pensamento dos filósofos que não estão comendo
int think(long i){
  printf("[%ld] - Pensando... (%d)\n", i, think_count[i]++);//incrementando a quantidade de quem está pensando
  sleep((int)i);//tempo do sleep de acordo com o índice da thread
}

//função que simula o acesso dos filósofos aos garfos
void* filosofo(void* p){
  long idx = (long)p;

  //se o index do filófo for igual a último garfo, então ele pode pegar, o último filósofo sempre tem que ter pelo menos um garfo
  if(idx == (QUANTIDADE_THREADS - 1)){
    while(running){
      sem_wait(&garfos[0]);//bloquendo o acesso ao primeiro garfo
      sem_wait(&garfos[QUANTIDADE_THREADS - 1]);//bloqueando o acesso ao último garfo
      eat(idx);//filósofo comendo
      sem_post(&garfos[0]);//liberando o acesso ao primeiro garfo
      sem_post(&garfos[QUANTIDADE_THREADS - 1]);//liberando o acesso ao último garfo
      think(idx);//após comerem filósofos começam a pensar
    }
  } else {//se o index não for igual ao último garfo então o filósofo pode pegar os outros garfos disponíveis
    while(running){
      sem_wait(&garfos[idx]);//bloquendo o acesso ao garfo atual
      sem_wait(&garfos[idx + 1]);//bloqueando o acesso ao próximo garfo
      eat(idx);//filósofo comendo
      sem_post(&garfos[idx]);//liberando o acesso ao garfo atual
      sem_post(&garfos[idx + 1]);//liberando o acesso ao próximo garfo
      think(idx);//após comerem filósofos começam a pensar
    }
  }
}

int main(void){
  //loop for para inicializar os semáforos para cada garfo que garante o acesso do filósofo a ele
  for(int i = 0; i < QUANTIDADE_THREADS; i++){
    sem_init(&garfos[i], 0, 1);
    eat_count[i] = 0;
    think_count[i] = 0;
  }

  //loop for para criar as threads
  for(long i = 0; i < QUANTIDADE_THREADS; i++){
    pthread_create(&phi[i], 0, filosofo, (void*)i);
  }

  sleep(30);
  running = 0;//após finalizar as atividades zera atribui zero a variável running

  //trazendo a execução das threads de volta para a main
  for(int i = 0; i < QUANTIDADE_THREADS; i++){
    pthread_join(phi[i], 0);
  }

  //destruindo os semáforos após a execução
  for(int i = 0; i < QUANTIDADE_THREADS; i++){
    sem_destroy(&garfos[i]);
  }

  return(0);
}