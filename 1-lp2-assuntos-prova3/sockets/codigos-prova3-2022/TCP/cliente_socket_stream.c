#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

/* SOCKET CLIENTE DO TIPO STREAM */
#define TAMANHO_MSG 512

//declarando a instância de acesso à estrutura do servidor
struct sockaddr_in destino;

//verificar se a conexão com o servidor foi bem sucedida
int conexao;

//declarando o FILE DESCRIPTOR do socket que será do tipo cliente
int e_socket;

int main(void){
  //criando o socket do tipo STREAM (TCP)
  e_socket = socket(AF_INET, SOCK_STREAM, 0);

  //verificando se foi bem sucedido a criação do socket
  if(e_socket < 0){
    perror("Erro na criação do socket()!\n");
    exit(1);
  }

  //preenchendo os dados dos servidor
  destino.sin_family = AF_INET;//informando que está fazendo uso da Pilha IP
  destino.sin_port = htons(4000);//informando a porta de conexão 
  destino.sin_addr.s_addr = inet_addr("127.0.0.1");//informando o endereço de conexão (loop back - referenciando a própria máquina)

  //função que cria conexão entre o socket do cliente com o socket do servidor, cujo endereço está nos parâmetros
  //a função connect() fica bloqueada até obter uma resposta do servidor
  conexao = connect(e_socket, (struct sockaddr*)&destino, sizeof(destino));

  //verificando se deu erro na criação da função connect()
  if(conexao < 0){
    perror("Erro na criação da função connect() - A PORTA 3000 ESTA FECHADA!!\n");
    close(e_socket);//fecha a conexão do socket()
    exit(1);
  }

  while(1){
    //alocando a memória da mensagem que o cliente vai enviar ao servidor
    char *mensagem = (char*) malloc(TAMANHO_MSG * sizeof(char) + 1);

    //solicitando mensagem ao cliente
    printf("[CLIENTE] - Digite uma mensagem: \n");
    scanf(" %s", mensagem);

    //se o cliente digitar exit o programa é encerrado
    if(strcmp(mensagem, "exit") == 0){
      close(e_socket);
      printf("Saindo...\n");
      free(mensagem);
      exit(0);
    }

    //função send() que envia mensagem do cliente para o servidor
    sendto(e_socket, (void*)mensagem, strlen(mensagem), 0, &destino, sizeof(destino));

    //buffer para armazenar que o servidor envia ao cliente
    char recv_buffer[TAMANHO_MSG];

    //função recv() que lê a mensagem enviada do servidor para o cliente
    recvfrom(e_socket, &recv_buffer, TAMANHO_MSG, 0, &destino, sizeof(destino));

    //exibindo mensagem no lado do cliente que foi enviada do servidor
    printf("Recebi do servidor: %s\n", recv_buffer);
    free(mensagem);

    //buffer para armazenar a mensagem que será enviada do cliente para o servidor
    // char* send_buffer = "ALO BRASIL - CLIENTE\n";

    //fechando o socket
    // close(e_socket);
  }

  return(0);
}