#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

/* SOCKET SERVIDOR DO TIPO STREAM */

//declarando estrutura do servidor
struct sockaddr_in servidor;

//declarando estrutura do cliente
struct sockaddr_in cliente_info;

//criar o socket FILE DESCRIPTOR
int s_socket;

//guarda o resultado da função accept() que ao aceitar uma conexão com cliente habilita esse socket para ser cliente
int c_socket;

//armazenar o tamanho da estrutura do cliente
int cinfo_len = 0;

int main(void){
  //realizando a criação do socket
  s_socket = socket(AF_INET, SOCK_STREAM, 0);

  //verificando se a criação do socket foi bem sucedido
  if(s_socket < 0){
    perror("Erro na criação do socket()!\n");
    exit(1);
  }

  //preenchendo a estrutura do sevidor 
  servidor.sin_family = AF_INET; //fazendo uso da Pilha IP
  servidor.sin_port = htons(4000); //informando a porta de conexão
  servidor.sin_addr.s_addr = INADDR_ANY;//informando ao Sistema Operacional que é um servidor e está aguardando informações

  //associando os dados do servidor ao socket criado
  int bind_result = bind(s_socket, (struct sockaddr*)&servidor, sizeof(servidor));

  //verificando se a criação do bind() foi bem sucedido
  if(bind_result != 0){
    perror("Erro na criação do bind()!\n");
    exit(1);
  }

  //função listen() habilitando o socket para receber conexões
  int listen_result = listen(s_socket, 10);

  //verificando se a criação da função listen() foi bem sucedido
  if(listen_result != 0){
    perror("Erro na criação do listen()!\n");
    exit(1);
  }

  //recebendo a solicitação de conexão do cliente e passando a estrutura do cliente que será preenchida assim que ele s conectar
  //habilitando c_socket para ser um socket de comunicação com o cliente
  c_socket = accept(s_socket, (struct sockaddr*)&cliente_info, &cinfo_len);

  //verificando se o accept() foi bem sucedido 
  if(c_socket < 0){
    perror("Erro na criação do accept()\n");
    exit(1);
  }

  //exibindo mensagem do lado do servidor
  printf("Recebi uma conexão do cliente!\n");

  //buffer para enviar mensagem ao cliente
  char* send_buffer = "ALO BRASIL - SERVIDOR\n";

  //enviando mensagem para o cliente
  send(c_socket, send_buffer, strlen(send_buffer), 0);

  //buffer para armazenar mensagens do cliente que é enviada para o servidor
  char buffer[40];

  //lendo mensagens que o cliente envia
  recv(c_socket, buffer, 40, 0);

  //exibindo mensagem no lado do servidor que foi enviada pelo cliente
  printf("Recebi do cliente: %s\n", buffer);

  //fechando socket do cliente
  close(c_socket);
  close(s_socket);

  return(0);
}