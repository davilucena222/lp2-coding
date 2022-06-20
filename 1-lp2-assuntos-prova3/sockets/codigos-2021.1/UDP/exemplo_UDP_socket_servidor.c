#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

/* SERVIDOR UDP - UMA PORTA SÓ É UTILIZADA PARA VOCÊ OUVIR E OUTRA PORTA PARA VOCÊ ESCREVER 
   AO TESTAR CLIENTES UDPs ELES NÃO MORREM DEVIDO NÃO TEREM CONEXÃO E SIM AGUARDO DA INTERAÇÃO DO USUÁRIO
*/
int running = 1;
int my_port = 0;

#define BUFF_SIZE 256

int main(void){
  int sock_fd, resp_socket_fd; //socket a ser criado tanto para os servidor quanto para o cliente (socket que vai ouvir, socket que vai responder)
  char buffer[BUFF_SIZE]; //buffer da mensagem
  struct sockaddr_in srv_addr, client_addr, resp_addr; //estrutura de endereçamento do servidor, do cliente que enviou e da resposta que será enviada 

  int addr_len; //tamanho do endereço
  int read_len; //tamanho do que será lido pelo servidor

  int client_port; //porta do cliente

  //variáveis para armazenar a data e enviar como resposta para o outro cliente
  char day[3], month[3], year[3];
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  sprintf(day, "%02d", t -> tm_mday, sizeof(day));
  sprintf(month, "%02d", t-> tm_mon + 1, sizeof(month));
  sprintf(year, "%04d", t -> tm_year + 1901, sizeof(year));

  //usuário fornece a porta
  printf("Digite a porta para ouvir UDP: ");  
  scanf("%d", &my_port);
  client_port = my_port + 1;

  //criando o socket do servidor UDP
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock_fd < 0){ //verificando se tem erro
    printf("Error ao criar o socket!");
    exit(0);
  }
  //zerando a estrutura do servidor
  bzero(&srv_addr, sizeof(struct sockaddr_in));
  
  //preenchendo a estrutura do servidor
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = INADDR_ANY; //especificando que vai receber dados 
  srv_addr.sin_port = htons(my_port);

  //associando as informações da estrutura do servidor ao socket 
  if(bind(sock_fd, &srv_addr, sizeof(srv_addr)) < 0){
    printf("Erro no bind()!\n");
    exit(0);
  }

  //tamanho da estutura do cliente
  addr_len = sizeof(client_addr);

  while(running){
    char msg_to_client[40];//buffer com mensagem para o cliente

    //servidor recebendo a mensagem do cliente
    //o mesmo socket que o servidor utiliza para receber mensagens do cliente é o mesmo que o cliente utiliza para enviar mensagens ao servidor (tudo isso devido ser protocolo UDP)
    read_len = recvfrom(sock_fd, (char *)buffer, BUFF_SIZE, MSG_WAITALL, (struct sockaddr *)&client_addr, &addr_len);

    //fechando a string e exibindo a mensagem do cliente no lado do servidor
    buffer[read_len + 1] = '\0';
    printf("Recebi do cliente: %s\n", buffer);

    //se o usuário digitar zero no buffer o servidor é encerrado
    if(strncmp("exit", buffer, 4) == 0){
      printf("Saindo do servidor...\n");
      running = 0;//fecha o while()
    }

    //primeiro cliente enviando mensagem para o outro cliente que foi criado (client_port)
    sprintf(msg_to_client, "%s / %s / %s\n", day, month, year);

    //preenchendo a estrutura do novo socket para enviar a resposta (client_port)
    resp_addr.sin_family = AF_INET;
    resp_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //adequado seria pegar pelo client_addr
    resp_addr.sin_port = htons(client_port);

    //criando o novo socket
    resp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    //enviando a mensagem de volta ao novo cliente por uma outra porta (client_port = my_port + 1)
    sendto(resp_socket_fd, msg_to_client, strlen(msg_to_client), 0, (struct sockaddr_in *)&resp_addr, sizeof(resp_addr));
  }

  return(0);
}