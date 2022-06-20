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
  O SERVIDOR NÃO PRECISA DAS INFORMAÇÕES COM QUEM ELE VAI SE CONECTAR, PELO CONTRÁRIO ELE VAI AGUARDAR APENS CONEXÕES EM UMA PORTA ESPECÍFICA QUE USUÁRIO VAI FONERCER

  ESTE EXEMPLO TRATA DE VÁRIOS CLIENTES FAZENDO USO DE THREAD, OU SEJA, SE VÁRIOS CLIENTES TENTAREM SE CONECTAR A UMA MESMA PORTA, TODOS ELES IRÃO OBTER SUCESSO
 */

#define TAM_MSG 50
#define QTD_CLIENTS 10
#define ADDR_SIZE 16

int client_counter = 0;//contador para os sockets dos clientes

void handle_client(void *p){
  //FILE DESCRIPTOR de um cliente
  int my_client_fd = (int)p;

  //servidor envia para o cliente a string "BEMVINDO"
  char *welcome_msg = "BEMVINDO\n";

  //mensagem para o cliente que o serrvidor enviará (assim que ele se conectar)
  //escreve para o cliente através do socket do cliente
  /* TRATANDO O CLIENTE */
  write(my_client_fd, welcome_msg, strlen(welcome_msg));

  //o servidor vai ficar lendo o que o cliente envia
  while(1){
    char buffer[TAM_MSG];//buffer para guardar mensagem que será enviada ao servidor através do cliente
    char msg_to_client[TAM_MSG];//buffer para armazenar mensagem que será enviada ao cliente através do servidor
    int read_size;//armazena o resultado de read()
    
    //função read() - apenas ler a mensagem de um socket (neste exemplo ele lê uma mensagem do cliente e exibe do lado do servidor)
    //read(socket do client criado pelo accetp após aceitar a conexão, buffer com a mensagem, tamnho da mensagem)
    read_size = read(my_client_fd, buffer, TAM_MSG);

    //se o read() for bem sucedido, a mensagem será exibida no lado do servidor
    //se a função read() for maior que zero, então houve sucesso, caso contrário não houve sucesso
    if(read_size > 0){
      //exibe mensgem do cliente no lado do servidor
      printf("Client: %s\n", buffer);

      //se o usuário digitar 0 no buffer então fecha o socket do cliente e encerra o processo utilizando um pthread_exit()
      if(strncmp("exit", buffer, 4) == 0){
        close(my_client_fd);
        client_counter--;
        pthread_exit(NULL);
      }
    }

    //servidor envia de volta para o cliente a quantidade de clientes que existem e estão conectados ao servidor
    sprintf(msg_to_client, "Contador clientes: %d\n", client_counter);
    //escrevendo a mensagem no socket do cliente
    //servidor utiliza a função write() para enviar a quantidade de clientes conectados ao servidor
    write(my_client_fd, msg_to_client, strlen(msg_to_client));
  }
}

int main(void){
  int server_fd;//armazena o socket do servidor (FILE DESCRIPTOR DO SERVIDOR)
  int client_fd[QTD_CLIENTS];//armazena o socket que vai se comunicar exclusivamente com cada cliente, ou seja, com cada thread (FILE DESCRIPTOR do cliente)
  int client_info_len[QTD_CLIENTS];//armazena o tamanho da estrutura do cliente

  unsigned int server_port = 0; //variável que vai armazenar a porta de comunicação entre servidor e cliente
  int client_index = 0; 

  //estrutura para preencher os campos do servidor
  struct sockaddr_in server_info;

  //estrutura pra preencher os campos do cliente
  struct sockaddr_in client_info[QTD_CLIENTS];

  //threads para os clientes (1 thread para cada cliente, cada thread terá uma struct específica)
  pthread_t client_thread[QTD_CLIENTS];

  //solicitando a porta do servidor
  printf("Digite a porta do servidor: ");
  scanf("%d", &server_port);

  //criando o socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  //verificando se houve erro na criação
  if(server_fd == -1){
    printf("Problemas na criação do socket!\n");
    exit(0);
  } else {
    //zerando a estrutura das informações do servidor
    bzero(&server_info, sizeof(server_info));

    //laço for para limpar a estrutura de todos os QTD_CLIENTS
    for(int i = 0; i < QTD_CLIENTS; i++){
      bzero(&client_info[i], sizeof(client_info[i]));
    }

    //preenchendo a estrutura do servidor
    server_info.sin_family = AF_INET; //informando o uso da pilha IP
    server_info.sin_addr.s_addr = htonl(INADDR_ANY); //informando ao Sistema Operacional que é um servidor e está aguardando informações (conexões)
    server_info.sin_port = htons(server_port); //capturando a porta de conexão  
    
    //assciando a estrutura que foi preenchida ao socket atravé da função bind()
    //associando as informações do FILE DESCRIPTOR ao servidor
    //bind(passa o socket, a estrutura com as informações do servidor, o tamanho dessa estrutura)
    //verificando se o bind() teve problemas
    //se o retorno da função bind() for igual a zero então significa que houve erro na criação da função
    //se for diferente de zero então houve sucesso
    if((bind(server_fd, (const struct sockaddr *)&server_info, sizeof(server_info))) != 0){
      printf("Problemas no bind()\n");
      exit(0);
    } else {//se o bind() não teve problemas então prossegue
      printf("Aguardando conexões na porta %d ", server_port);

      //listen(passa o socket, quantas conexões serão enfileiradas até que novas tentativas de conexões sejam dispensadas)
      //função listen() - informa qual o tamanho da pilha de conexões que o Sistema Operacional vai alocar
      //listen() - é onde o socket servidor é registrado para receber conexões
      //se o retorno da função listen() for igual a zero então significa que houve erro na criação da função
      //se for diferente de zero então houve sucesso
      if((listen(server_fd, QTD_CLIENTS)) != 0){
        printf("\nErro ao invocar função listen()!\n");
        exit(0);
      } else {
        printf(". . .\n");

        //buffer de endereço para ler o endereço do cliente
        char addr_buf[ADDR_SIZE + 1];


        //accept(passa o socket servidor da thred/cliente específico, endereço da estrutura que vai armazenar as informações do cliente em específico, ponteiro para inteiro específico do client/thread)
        //accept() é uma função bloqueante que só será desbloqueada se aceitar o cliente
        //se a função accept() retorna um número menor zero então houve erro na criação da função, caso contrário houve sucesso
        //quando a conexão do cliente é aceita pela função accept() um socket é criado para estabelecer a conexão com este cliente
        //laço while() para criar o accept() X vezes e manter a função main viva, além disso criar mais threads para executar a função handle_client()
        //o while() gera um cliente (thread) que vai executar a função handle_client() e se comunicar com o servidor, ou seja, um socket é criado para cada cliente
        //o loop while() fica recebendo conexões do cliente
        //accept() - também recebe informações do cliente e essas informações podem ser exibidas
        while(client_fd[client_index] = accept(server_fd, (struct sockaddr *)&client_info[client_index].sin_addr, &client_info_len[client_index])){

          //capturando o endereço do cliente pela struct que foi passada e exibindo
          char *client_ip_addr = inet_ntoa(client_info[client_index].sin_addr);
          printf("[%d] - Recebi conexão de %s\n", client_index, client_ip_addr);

          //criando as threads
          //passando o FILE DESCRIPTOR do socket como parâmetro da função handle_client()
          /* 
            * O TRATAMENTO DE CONEXÃO E COMUNICAÇÃO DE ENVIAR E RECEBER MENSAGENS ENTRE CLIENTE E SERVIDOR SERÁ PASSADO PARA A THREAD REALIZAR ATRAVÉS DE FUNÇÕES
         */
          //pthread_create(FILE DESCRIPTOR do cliente (socket do cliente), constante, função para a thread executar, index do cliente que será passada como parâmetro da função e realizará atividades)
          pthread_create(&client_thread[client_index], NULL, handle_client, (void *)client_fd[client_index]);

          client_counter++;//incrementando o counter na medida em que novas conexões de clientes são realizadas
          client_index++;//incrementando o cliente_index até a décima posição, que é a faixa de clientes declarados
          client_index = client_index % 10;

          //informando a quantidade total de clientes conectados
          printf("Total de %d clientes conectados\n", client_counter);
        }
      }
    }
  }

  return(0);
}       