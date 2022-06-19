#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

struct sockaddr_in servidor;
struct sockaddr_in client_info;

int s_socket, c_socket, cinfo_len=0;

int main(void){

	s_socket = socket(AF_INET,SOCK_STREAM,0);
	
	if(s_socket < 0) {
		perror("ERRO !");
		exit(1);
	}
	
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(31337);
	servidor.sin_addr.s_addr = INADDR_ANY;

	int bind_result = bind(s_socket, (struct sockaddr*) &servidor, sizeof(servidor));
	if (bind_result != 0) {
		perror("Bind error!");
		exit(1);
	}

	int list_result = listen(s_socket, 10);
	if (list_result != 0) {
		perror("Listen error!");
		exit(1);
	}
	
	c_socket = accept(s_socket, (struct sockaddr*)&client_info, &cinfo_len);

	if (c_socket < 0) { 
		perror("Accept error!");
		exit(1);
	}

	printf("Recebi uma conexao de um cliente!\n");
	//TODO: exibir client info
	//
	char* sbuf = "ALO BRASIL - SERVIDOR\n";
	
	send(c_socket, sbuf, strlen(sbuf), 0);

	char rbuf[40];

	recv(c_socket, rbuf, 40, 0);

	printf("Recebi: %s\n", rbuf);


	close(c_socket);
	close(s_socket);

	return 0;

}
