#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
// #define TAMANHO_THREADS 30

void novoProcesso(void){
	printf("Novo processo criado!\n");
}

void* novaThread(void *p) {
    printf("Nova thread criada!\n");
    return 0;
}

int main(void){
	int numeroDeProcessos;
	int pid_original;
	int fork_result;
	int i = 0, j = 0;
	// pthread_t n[TAMANHO_THREADS];
	pthread_t n;
	

	pid_original = getpid();

	printf("Informe a quantidade de processos: \n");
	scanf("%d", &numeroDeProcessos);
	numeroDeProcessos--;

	for(i = 0; i < numeroDeProcessos; i++){
		if(pid_original == getpid()){
			fork_result = fork();
			sleep(2);
		}
	}

	novoProcesso();
	pthread_create(&n, NULL, novaThread, NULL);

	return(0);
}