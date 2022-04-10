#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
	printf("[ex] Sou o exec-externo!!!\n");
	sleep(20);
	printf("[ex] Saindo...\n");
	
	return(0);
}