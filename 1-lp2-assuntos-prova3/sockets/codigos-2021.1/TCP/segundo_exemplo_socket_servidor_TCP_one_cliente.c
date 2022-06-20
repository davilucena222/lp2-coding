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

/*
  CRIANDO UM SOCKET DO TIPO SERVIDOR TCP
  O SERVIDOR NÃO PRECISA DA INFORMAÇÕES COM QUEM ELE VAI SE CONECTAR, PELO CONTRÁRIO ELE VAI AGUARDAR APENS CONEXÕES EM UMA PORTA ESPECÍFICA QUE USUÁRIO VAI FONERCER

  ESTE EXEMPLO TRATA APENAS UM CLIENTE, SE HOUVER 2 CLIENTES TENTANDO SE CONECTAR A MESMA PORTA UM DOS DOIS VAI FICAR EM ESPERA ATÉ O OUTRO DÁ O LUGAR
 */

#define TAM_MSG 50

int main(void){
  int sock_fd;//armazena o socket do servidor
  int client_fd;//armazena o socket que vai se comunicar exclusivamente com o cliente
  int client_info_len;//armazena o tamanho da estrutura do cliente
  unsigned int server_port = 0;

  //estrutura para preencher os campos do servidor
  struct sockaddr_in server_info;

  //estrutura pra preencher os campos do cliente
  struct sockaddr_in client_info;

  printf("Digite a porta do servidor: ");
  scanf("%d", &server_port);

  //criando o socket
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  //verificando se houve erro na criação
  if(sock_fd == -1){
    printf("Problemas na criação do socket!\n");
  } else {
    //zerando a estrutura
    bzero(&server_info, sizeof(server_info));
    bzero(&client_info, sizeof(client_info));

    //preenchendo a estrutura do servidor
    server_info.sin_family = AF_INET; //informando o uso da pilha IP
    server_info.sin_addr.s_addr = htonl(INADDR_ANY); //informando ao Sistema Operacional que é um servidor e está aguardando informações
    server_info.sin_port = htons(server_port); //capturando a porta de conexão  
    
    //assciando a estrutura que foi preenchida ao socket atravé da função bind()
    //bind(passa o socket, a estrutura com as informações do servidor, o tamanho dessa estrutura)
    //verificando se o bind() teve problemas
    //se o retorno da função bind() for igual a zero então significa que houve erro na criação da função
    //se for diferente de zero então houve sucesso
    if((bind(sock_fd, &server_info, sizeof(server_info))) != 0){
      printf("Problemas no bind()\n");
      exit(0);
    } else {//se o bind() não teve problemas então prossegue
      printf("Aguardando conexões na porta %d ", server_port);

      //listen(passa o socket, quantas conexões serão enfileiradas até que novas tentativas de conexões sejam dispensadas)
      //função listen() - informa qual o tamanho da pilha de conexões que o Sistema Operacional vai alocar
      //se o retorno da função listen() for igual a zero então significa que houve erro na criação da função
      //se for diferente de zero então houve sucesso
      if((listen(sock_fd, 5)) != 0){
        printf("\nErro ao invocar função listen()!\n");
        exit(0);
      } else {
        printf(". . .\n");

        //accept(passa o socket servidor, endereço da estrutura que vai armazenar as informações do cliente, ponteiro para inteiro)
        //accept() é uma função bloqueante que só será desbloqueada se aceitar o cliente
        //se a função accept() retorna um número menor zero então houve erro na criação da função, caso contrário houve sucesso
        //quando a conexão do cliente é aceita pela função accept() um socket é criado para estabelecer a conexão com este cliente
        //client_fd - guarda o novo socket de comunicação
        client_fd = accept(sock_fd, (struct sockaddr *)&client_info, &client_info_len);
        if(client_fd < 0){
          printf("\nErro ao invocar o accept()!\n");
          exit(0);
        } else {
          printf("Cliente se conectou! - \n");

          //capturando e exibindo o IP do cliente
          char *client_ip_addr = inet_ntoa(client_info.sin_addr);
          printf("Endereço %s\n", client_ip_addr);

          //mensagem para o cliente
          char *server_welcome_msg = "BEMVINDO";

          //escrevendo no socket do cliente
          //write(socket do cliente, mensagem para o cliente, tamanho da mensagem)
          //será enviado para o cliente pelo socket
          write(client_fd, server_welcome_msg, strlen(server_welcome_msg));

          while(1){
            char buffer[TAM_MSG];//buffer para guardar mensagem que será enviada ao servidor através do cliente
            char msg_to_client[TAM_MSG];//buffer para armazenar mensagem que será enviada ao cliente através do servidor
            int read_size;//armazena o resultado de recv()
            
            //função recv() apenas ler a mensagem de um socket (neste exemplo ele lê uma mensagem do cliente e exibe do lado do servidor)
            //recv(socket do client criado pelo accetp após aceitar a conexão, buffer com a mensagem, tamnho da mensagem, constante)
            read_size = recv(client_fd, buffer, TAM_MSG, 0);

            //se o recv() for bem sucedido, a mensagem será exibida no lado do servidor
            //se função recv() maior que zero, então houve sucesso, caso contrário não houve sucesso
            if(read_size > 0){
              printf("Client: %s\n", buffer);
            }

            //solicitando mensagem ao servidor para enviar ao cliente
            printf("Digite uma mensagem para enviar para o cliente: ");
            scanf("%s", msg_to_client);

            //se o usuário digitar zero no exit o processo do servidor será encerrado
            if(strcmp("exit", msg_to_client) == 0){
              //fecha o servidor
              close(sock_fd);
              exit(0);
            }

            //escrevendo a mensagem no socket do cliente
            write(client_fd, msg_to_client, strlen(msg_to_client));
          }
        }
      }
    }
  }

  return(0);
}