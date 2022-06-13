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

//definindo porta para enviar mensagem
#define PORT 9899

//tamanho do buffer
#define TAM_BUFFER 512

int main(void){
    //declarando socket (file descriptor)
    int sockfd;

    //declarando buffer e mensagem
    char buffer[TAM_BUFFER];
    char* my_msg = "HELLO DO CLIENTE";

    //declarando struct para preencher
    struct sockaddr_in servaddr;

    //criando o socket DATAGRAMA
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //verificando se tem erro
    if(sockfd < 0){
        perror("socket error!\n");
        exit(EXIT_FAILURE);
    }

    //preechendo os dados do servidor
    servaddr.sin_family = AF_INET; //sinalizando que está utilizando a pilha IP
    servaddr.sin_port = htons(PORT); //sinalizando para qual porta o cliente deverá se comunicar (mandar a mensagem)
    servaddr.sin_addr.s_addr =  inet_addr("127.0.0.1"); //endereço do servidor (loop back - referenciando a própria máquina)

    //omitindo o bind pois no sendto() é passada todas as informações que estariam no bind()
    /*int bind_result = bind(sockfd, (const struct sockaddr_in*)&servaddr, sizeof(servaddr));

    if(bind_result < 0){
        perror("bind error!\n");
        exit(EXIT_FAILURE);
    }*/

    //enviando mensagem para o servidor
    sendto(sockfd, (char*)my_msg, strlen(my_msg), MSG_CONFIRM, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("Mensagem enviada ao servidor: %s\n", my_msg);

    int recv_len;
    int serv_len;
/*
    //aguardando a mensagem do servidor
    recv_len = recvfrom(sockfd, (char*)buffer, TAM_BUFFER, MSG_WAITALL, (struct sockaddr*)&servaddr, &serv_len);

    //fechando a string do buffer
    buffer[recv_len] = '\0';

    printf("Recebi do servidor: %s\n", buffer);
*/
    return(0);
}