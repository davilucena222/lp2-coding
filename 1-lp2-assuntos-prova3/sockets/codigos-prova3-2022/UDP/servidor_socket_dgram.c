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

/* SERVIDOR SOCKET DO TIPO DGRAM */ 
/* COMANDO PARA EXECUTAR CLIENTE: nc -u localhost 9001 */

//definindo porta
#define PORT 7000

//definindo tamanho do buffer para a mensagem de comunicação
#define TAMANHO_BUFFER 512

int main(void) {
  //FILE DESCRIPTOR para criar e referenciar o socket
  int sock_fd;

  //string para receber mensagens do cliente
  char buffer[TAMANHO_BUFFER];

  //criando a instância para referenciar a estrutura com os dados do servidor
  struct sockaddr_in serv_addr;

  //criando a instância para referenciar a estrutura com os dados do cliente (quando o cliente se conecta ao servidor então é possível preencher tal estrutura com os seus dados)
  struct sockaddr_in cli_addr;

  //criando o socket do tipo DGRAM (UDP)
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  //verificando se a criação do socket() deu certo
  if(sock_fd < 0){
    perror("Erro na criação do socket()!\n");
    exit(EXIT_FAILURE);
  }

  //preenchendo a estrutura do servidor
  serv_addr.sin_family = AF_INET;//informando apenas que está fazendo uso da Pilha IP
  serv_addr.sin_addr.s_addr = INADDR_ANY;//sinalizando que é um socket servidor e que vai aguardar informação de qualquer lugar
  serv_addr.sin_port = htons(PORT);//passando a porta aguardada a mensagem do cliente

  //associar o socket a estrutura do servidor
  int bind_result = bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  //verificando se a criação do bind() deu certo
  if(bind_result < 0){
    perror("Erro na criação do bind()!\n");
    exit(EXIT_FAILURE);
  }

  /* SERÁ REALIZADO O RECEBIMENTO E ENVIO DE MENSAGENS ENTRE SERVIDOR E CLIENTE */
  
  int cliaddr_len;//armazenando o tamanho da estrutura do cliente
  int recv_len;//recv_len detém o tamanho da mensagem que o cliente enviará

  //mensagem exibida do lado do servidor
  printf("AGUARDANDO MENSAGEM DO CLIENTE...\n");

  //criando função do servidor que vai aguardar mensagens do cliente e armazenar essa mensagem no buffer
  //a variável recv_len detém o tamanho da mensagem que o cliente enviará
  recv_len = recvfrom(sock_fd, (char*)buffer, TAMANHO_BUFFER, MSG_WAITALL, (struct sockaddr*)&cli_addr, &cliaddr_len);

  //delimitando o final da mensagem que o cliente enviou e foi recebida pelo servidor
  buffer[recv_len + 1] = '\0';

  //exibindo a mensagem do lado do servidor
  printf("Recebi do cliente 1: %s\n", buffer);

  recv_len = recvfrom(sock_fd, (char*)buffer, TAMANHO_BUFFER, MSG_WAITALL, (struct sockaddr*)&cli_addr, &cliaddr_len);

  buffer[recv_len + 1] = '\0';

  printf("Recebi do cliente 2: %s\n", buffer);


  //exibindo no lado do servidor
  printf("Finalizando...\n");

  // sleep(2);

  return(0);
}