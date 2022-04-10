#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

int main(void) {
	int fork_result;
	
	char *args[] = {"./programa-externo", NULL};
	
	printf("[i-po] Antes do exec...\n");
	fork_result = fork();

	if(fork_result == 0){
		//processo clonado
		printf("[i-pc] antes do exec...\n");
		sleep(5);
		//substitui o processo clonado pelo processo do "programa externo"
		//função execvp substitui a execução do processo atual por outro
		execvp(args[0], args);
	} else {
		//processo original
		printf("[i-po] Depois do exec...\n");
		sleep(10);
	}
	
	return(0);
}
