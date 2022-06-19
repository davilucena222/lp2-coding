#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

struct sockaddr_in destino;
int conexao;
int e_socket;

int main(void){

	e_socket = socket(AF_INET,SOCK_STREAM,0);
	
	if(e_socket < 0) {
		perror("ERRO !");
		exit(1);
	}
	
	destino.sin_family = AF_INET;
	destino.sin_port = htons(31337);
	destino.sin_addr.s_addr = inet_addr("127.0.0.1");
	//bzero(&(destino.sin_zero),8);
	//
	conexao = connect(e_socket,(struct sockaddr * )&destino, sizeof(destino));
	
	if(conexao < 0) {
		perror("A PORTA 31337 ESTA FECHADA!\n"); 
		close(e_socket);
		exit(1);
	}
	
	char rbuf[40];
	recv(e_socket, rbuf, 40, 0);	
	printf("Recebi: %s\n", rbuf);

	char* sbuf = "ALO BRASIL - CLIENTE\n";
	send(e_socket, sbuf, strlen(sbuf), 0);



	close(e_socket);

	return 0;

}
