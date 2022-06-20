#include <stdio.h>
#include <pthread.h>

//estrutura do monitor
typedef struct {
  int id_cliente;
  double saldo;
  pthread_mutex_t mutex;
} conta_corrente;

//função que retorna o saldo atual
float get_saldo(conta_corrente *cc){
  return cc -> saldo;
}

//função que vai alterar o saldo
void altera_saldo(conta_corrente *cc, float valor){
  //bloqueando o acesso à sessão crítica (garantindo a exclusão mútua)
  //trava - pelo menos 1 thread passa para a seção crítica
  pthread_mutex_lock(&cc -> mutex);
  cc -> saldo += valor;
  //liberando o acesso à sessão crítica
  pthread_mutex_unlock(&cc -> mutex);
}

conta_corrente cc1, cc2;

//função para realizar outras operações
void *operacoes1(void *p){
  unsigned int counter  = 0;
  while(counter < 9999999){
    altera_saldo(&cc1, 100); //passando o valor para alterar o saldo
    printf("[c] %f\n", cc1.saldo);
    counter++;
  }
}

//função para realizar outras operações
void *operacoes2(void* p){
  unsigned int counter  = 0;
  while(counter < 9999999){
    altera_saldo(&cc1, -100); //passando o valor para alterar o saldo
    printf("[c] %f\n", cc1.saldo);
    counter++;
  }
}

int main(void){
  //threds para manipular as funções
  pthread_t thread1, thread2;

  //threads realizando as operações no saldo
  pthread_create(&thread1, NULL, operacoes1, NULL);
  pthread_create(&thread2, NULL, operacoes2, NULL);

  //trazendo a execução das threads de volta para a main
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("Saldo final: %.2lf\n", cc1.saldo);

  return(0);
}