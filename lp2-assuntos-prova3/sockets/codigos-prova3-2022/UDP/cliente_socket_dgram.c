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

/* SOCKET CLIENTE DO TIPO DGRAM 
   POR SER UM SOCKET CLIENTE DO TIPO DGRAM NÃO HÁ GARANTIA DE ENTREGA DA MENSAGEM TANTO POR PARTE DO CLIENTE QUANTO POR PARTE DO SERVIDOR, TUDO ISSO POR SE TRATAR DE UM PROTOCOLO UDP
*/

//definindo porta para enviar mensagens
#define PORT 7000

//definindo o tamanho do buffer
#define TAMANHO_BUFFER 512

int main(void){
  //declarando socket (FILE DESCRIPTOR)
  int sockfd;

  //declarando buffer e mensagem para enviar ao servidor
  char buffer[TAMANHO_BUFFER];
  char *my_msg = "HELLO DO SEU CLIENTE!";

  //criando instância da estrutura do servidor UDP
  struct sockaddr_in servaddr;

  //criando o socket do tipo DGRAM
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  //verificando se deu certo a criação do socket()
  if(sockfd < 0){
    perror("Erro na criação do socket()!\n");
    exit(EXIT_FAILURE);
  }

  //preenchendo os dados do servidor
  servaddr.sin_family = AF_INET;//sinalizando que está utilizando a pilha IP
  servaddr.sin_port = htons(PORT);//sinalizando para qual porta o cliente deverá se comunicar (mandar a mensagem)
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");////endereço do servidor (loop back - referenciando a própria máquina como cliente)

  //função sendto() para enviar mensagem ao servidor
  sendto(sockfd, (char*)my_msg, strlen(my_msg), MSG_CONFIRM, (struct sockaddr*)&servaddr, sizeof(servaddr));

  //exibindo mensagem do lado do cliente (mensagem que foi enviada ao servidor)
  printf("Mensagem enviada ao servidor: %s\n", my_msg);

  return(0);
}