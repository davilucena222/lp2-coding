#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/**************** SOCKET DATAGRAMA *****************/

//definindo porta para aguardar mensagem
#define PORT 9899

//definindo o buffer para se comunicar
#define TAM_BUFFER 512

int main(void){
    int sockfd; //file descriptor para referenciar o socket
    char buffer[TAM_BUFFER]; //string para receber alguma informação do cliente

    char *hello_msg = "HELLO DO SERVIDOR"; //mensagem para o cliente 

    //programador preenche a variável servaddr e recvfrom preenche a variável do cliente
    struct sockaddr_in servaddr, cliaddr; //duas variáveis do tipo struct que fazem parte do cabeçalho

    //criando o socket e armazenando o falor de retorno
    //AF_INET = pilha ip
    //SOCK_DGRAM = tipo do socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //verificando se tem erro
    if(sockfd < 0){
        perror("socker() error!\n");
        exit(EXIT_FAILURE);
    }
    
    //passando as informações do servidor para aguardar mensagens de uma determinada porta
    servaddr.sin_family = AF_INET; //sinalizando que está utilizando a pilha IP
    servaddr.sin_addr.s_addr = INADDR_ANY; //sinalizando que é um socket servidor (receberá uma informação de qualquer lugar)
    servaddr.sin_port = htons(PORT); //porta que será aguardada a mensagem

    //associando a estrutura ao socket
    int bind_result = bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    //verificando se tem erro
    if(bind_result < 0){
        perror("bind() error\n");
        exit(EXIT_FAILURE);
    }


    int cliaddr_len;
    int recv_len;

    printf("AGUARDANDO MENSAGEM DO CLIENTE\n");

    //preenchendo as informações do cliente
    //aguardando a mensagem do cliente pelo recvfrom()
    //passando a estrutura vazia e preenchendo
    recv_len = recvfrom(sockfd, (char*)buffer, TAM_BUFFER, MSG_WAITALL, (struct sockaddr*)&cliaddr, &cliaddr_len);

    //fechando a string do buffer
    buffer[recv_len] = '\0';

    printf("Recebi do Cliente 1: %s\n", buffer);

    recv_len = recvfrom(sockfd, (char*)buffer, TAM_BUFFER, MSG_WAITALL, (struct sockaddr*)&cliaddr, &cliaddr_len);

    buffer[recv_len] = '\0';

    printf("Recebi do Cliente 2: %s\n", buffer);

/*
    //mandando a mensagem de volta ao cliente
    sendto(sockfd, (char*)hello_msg, strlen(hello_msg), MSG_CONFIRM, (struct sockaddr*)&cliaddr, cliaddr_len);
*/

    printf("Finalizando...\n");

    sleep(2);

    return(0);
}