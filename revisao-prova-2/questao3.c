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

int lock = 0;
int lockOperacao = 0;
int debito = 0;
int credito = 0;
int saldo = 0;

#define QTD_THREADS 5

void* operacaoBancariaDebitoCredito(void* p){
  long index = (long)p;
  int totalDebito = 0;
  int totalCredito = 0;

  while(__atomic_test_and_set(&lock, 1));
  printf("[%ld] - Iniciando operações...\n", index);
  saldo = rand() % 10000;
  printf("[%ld] - Valor gerado para o saldo: %d\n", index, saldo);
  for(int i = 0; i < QTD_THREADS; i++){
    debito = rand() % 500;
    credito = rand() % 500;
    
    totalDebito += debito;
    totalCredito += credito;
    
    saldo = saldo - debito - credito;
    debito = 0;
    credito = 0;
  }
  printf("[%ld] - Valor total retirado em compras no débito: %d\n", index, totalDebito);
  printf("[%ld] - Valor total retirado em compras no crédito: %d\n", index, totalCredito);
  printf("[%ld] - saldo restante: %d\n", index, saldo);
  puts(".."); 
  lock = 0;
}

int main(void){
  pthread_t threads[QTD_THREADS];
	time_t tempo;
  srand(time(&tempo));

  for(long i = 0; i < QTD_THREADS; i++){
    sleep(1);
    pthread_create(&threads[i], NULL, operacaoBancariaDebitoCredito, (void*)i);
  }

  for(int i = 0; i < QTD_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  return(0);
}