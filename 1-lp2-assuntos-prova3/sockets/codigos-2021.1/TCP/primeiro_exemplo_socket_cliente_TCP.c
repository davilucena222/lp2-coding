#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define TAM_STR_HOST 100
#define TAM_MSG 100

/* CRIANDO UM SOCKET DO TIPO CLIENTE TCP */

int main(void) {
	int meu_socket_stream; //variável para o socket stream (TCP)
	int connection_result; //armazena o resultado da função connect
	int porta_servidor; //variável para guardar a porta de comunicação
	char host_servidor[TAM_STR_HOST]; //variável para armazenar o servidor

	//estrutura definida na API de sockets (criando uma instância dessa estrutura chamada endereço)
	//essa estrutura contém campos relacionados ao endereço do servidor sobre o qual será conectado
	struct sockaddr_in endereco_srv;

	//capturar o endereço da recepção das informações quando os pacotes são recebidos do servidor
	//estrutura para o recvfrom() que será preenchida pelos dados do servidor ao receber uma mensagem
	struct sockaddr_in endereco_recv;

	//mensagem para o servidor
	char *msg;

	//mensgem que será recebida do servidor
	char msg_recv[TAM_MSG];

	printf("Digite o endereço do servidor: \n");
	gets(host_servidor);

	printf("Digite a porta usada na conexão: \n");
	scanf("%d", &porta_servidor);

	//criando o socket
	//parâmetros (informar que vai utilizar o protocolo IP, informar se é socket datagram ou stream, último parâmetro sempre 0)
	meu_socket_stream = socket(PF_INET, SOCK_STREAM, 0);//TCP

	//zerando tudo na variável (região de memória que contém lixo)
	bzero(&endereco_srv, sizeof(endereco_srv));

	//preenchendo campos da estrutura do servidor (informações as quais que o cliente precisa saber para se conectar)
	endereco_srv.sin_family = AF_INET; //informando que vai utilizar a pilha IP
	endereco_srv.sin_port = htons(porta_servidor); //modificando um inteiro de 4 bytes para 2 bytes
	inet_aton(host_servidor, &endereco_srv.sin_addr); //(passando o servidor, e a estrutura em que os servidor será armazenado)

	//se conectando ao servidor 
	//connect(passando o File Descriptor ou socket, endereço da estrutura, tamanho da estrutura)
	//função connect() retorna um valor inteiro
	connection_result = connect(meu_socket_stream, &endereco_srv, sizeof(endereco_srv));

	//verificando se tem erro
	if(connection_result == 0){
		printf("Conectado ao servidor %s na porta %d\n", host_servidor, porta_servidor);
	} else {
		printf("Falha na conexão com o servidor %s\n", host_servidor);
		exit(0);
	}

	while(1){
		//alocando a variável da mensagem para livrar ela quando quiser enviar outra mensagem
		msg = (char*) malloc(TAM_MSG * sizeof(char) + 1);//'+1' - devido ser uma string

		printf("Digite uma mensagem: \n");
		scanf(" %s", msg);
		// gets(msg);

		if(strcmp(msg, "exit") == 0){
			close(meu_socket_stream); //fechando o socket se o valor de msg for zero
			printf("Saindo...\n");
			//mesmo que feche o servidor é preciso dar o free
			free(msg);
			exit(0);
		}

		//sendto(passa o socket,  buffer com a mensagem, o tamanho da mensgem/buffer, constante 0, instancia da estrutura servidor, tamanho da instância da estrutura servidor);
		//função que enviará mensagem ao servidor
		sendto(meu_socket_stream, (void *)msg, strlen(msg), 0, &endereco_srv, sizeof(endereco_srv));

		//recvfrom(passa o socket, o buffer com a mensgem, o tamanho da mensagem/buffer, a estrutura que será preenchida, tamanho da estrutura)
		//função que receberá a mensagem do servidor, só será desbloqueada se receber a mensagem 
		//recvfrom() fica bloquada enquanto não receber uma mensagem do clientes e ela não se perde, pois fica armazenada no espaço de memória reservado pelo Sistema Operacional
		recvfrom(meu_socket_stream, &msg_recv, TAM_MSG, 0, &endereco_recv, sizeof(endereco_recv));
		printf("Mensagem do servidor: %s\n", msg_recv);
		free(msg);
	}

	/* 
		nc - comando para criar um servidor que vai exibir as mensagens do cliente e quando digitar vai enviar mensagens para o cliente: nc -l -p 7777
		telnet - comando para criar um cliente: telnet localhost 7777
	*/

	return(0);
}
