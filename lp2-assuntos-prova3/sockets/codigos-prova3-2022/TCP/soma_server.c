#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>

//definindo quantidade de clientes/threads
#define QTD_CLIENTES 5

//definindo o tamanho do buffer
#define BUFFER_SIZE 16

//definindo a porta de comunicação
#define PORT 9997

//estrutura monitor para manipular o acesso a variável soma
typedef struct monitor {
  int soma;//variável para realizar os incrementos da soma
  sem_t mutex;//semáforo para bloquer e desbloquear acesso a variável soma
} t_monitor;

t_monitor monitor_soma;//instância para acessar a estrutura monitor

//função que acessa o atual valor de soma
int monitor_get_soma(void){
  return monitor_soma.soma;
}

//função que incrementa o valor de soma
//o valor de soma vai se acumulando
//apenas um cliente/thread acessa essa função por vez, exclusão mútua
void monitor_incrementa_soma(int n){
  sem_wait(&monitor_soma.mutex);//decrementando o valor do semáforo e a thread é posta para dormir
  //seção crítica  
  monitor_soma.soma += n; 
  sem_post(&monitor_soma.mutex);//incrementa o valor do semáforo e a thread que estava dormindo será acordada pois a seção crítica foi liberada
}

//funções que as threads/clientes vão executar
//como parãmetro a função recebe o socket do cliente que foi criado pela função accept()
void* handle_client(void *p){
  int cli_sock = (int)p;//recebe o socket do cliente

  //buffer para o cliente se comunicar com o servidor
  char buffer[BUFFER_SIZE + 1];

  //realizando a operação enquanto o cliente não digitar 0, pois se digitar zero encerra o programa
  //cliente incrementa a variável soma do monitor e o servidor devolve esse valor duplicado
  while(1){//loop while() fica vivo enquanto usuário não digitar zero 
  
    //servidor lê a mensagem do cliente
    read(cli_sock, buffer, BUFFER_SIZE);

    //delimitando o final da string
    buffer[BUFFER_SIZE] = '\0';

    //exibindo no lado do servidor o que foi enviado pelo cliente
    printf("Recebi do cliente: %s\n", buffer);

    //convertendo string para inteiro, pois o buffer é do tipo char e o número é salvo como char,para isso é preciso converter para então verificar se o número é igual a zero ou realizar o incremento na variável soma do monitor
    int num = atoi(buffer);

    //se o cliente digitar zero para o buffer então o servidor será encerrado
    if(num == 0){
      printf("Finalizando servidor...\n");
      exit(0);
    }

    //chamando a função para incrementar a variável soma da estrutura monitor e passando como parâmetro o valor a ser adicionado
    //a variável soma terá um valor incremetado por cada uma dos clientes
    monitor_incrementa_soma(num);

    //recuperando o valor que foi passado por cada cliente para a variável soma do monitor antes de duplicar através da função monitor_get_soma() e enviar para o servidor
    printf("Soma: %d\n", monitor_get_soma());

    //duplicando o valor que foi fornecido pelo cliente
    num *= 2;

    //armazenando o valor de num que foi duplicado
    char str_return[BUFFER_SIZE];

    //passando o valor de num duplicado para o buffer str_return
    sprintf(str_return, "%d\n", num);

    //demonstrando no lado do servidor o valor de num duplicado
    printf("Retornei %d\n", num);

    //enviando o valor duplicado para o cliente
    write(cli_sock, str_return, strlen(str_return));
  }
}

int main(void){
  int server_sock;//variável socket FILE DESCRIPTOR
  int cli_sock[QTD_CLIENTES];//variável com QTD_CLIENTES para preencher a estrutura de cada cliente 
  pthread_t cli_threads[QTD_CLIENTES];//declarando threads

  int clients = 0;//variável para referenciar a posição da thread específica 

  struct sockaddr_in server_addr;//instância para acessar e preencher a estrutura do servidor
  struct sockaddr_in cli_addr;//instância para acessar e preencher a estrutura do cliente
  int cli_addr_size;//variável para armazenar o tamanho da estrutura do cliente

  //inicializando o semáforo
  sem_init(&monitor_soma.mutex, 0, 1);

  //mensagem exibida no lado do servidor
  printf("Iniciando servidor de soma na porta: %d...!\n", PORT);

  //criando o socket do tipo TCP que garante a conexão
  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  //verificando se a criação do socket deu certo
  if(server_sock < 0){
    perror("socket() error!\n");
    exit(-1);
  }

  //preencher a estrutura do servidor
  server_addr.sin_family = AF_INET;//informando que está fazendo uso da Pilha IP
  server_addr.sin_addr.s_addr = INADDR_ANY;//sinaliando ao Sistema Operacional que está criando um servidor e que vai aguardar conexões
  server_addr.sin_port = htons(PORT);//informando a porta de comunicação

  //associando a estrutura do servidor ao socket que foi criado
  int bind_result = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

  //verificando se o bind() foi bem sucedido
  if(bind_result < 0){
    perror("bind() error!\n");
    exit(-1);
  }

  //função listen() habilita o socket para receber conexões
  int listen_result = listen(server_sock, QTD_CLIENTES);

  //verificando se a função listen() foi bem sucedida
  if(listen_result < 0){
    perror("listen() error!\n");
    exit(-1);
  }

  //loop infinito que habilita aguardar conexões de clientes e interações com os servidor
  while(1){
    //função accept() que aguarda conexões de clientes e fica bloqueada enquanto não aceita uma conexão
    //quando a função accept() aceita conexões sockets são criados para os clientes poderem se comunicarem
    //função accept() ao aceitar clientes a estrutura do cliente é preenchida com as informações referentes a ele
    int clisock = accept(server_sock, (struct sockaddr*)&cli_addr, &cli_addr_size);

    //verificando se a criação do socket do cliente foi bem sucedida
    if(clisock < 0){
      perror("accept() error!\n");
      exit(-1);
    }

    //exibindo para o servidor que um novo cliente foi conectado
    printf("Novo cliente conectado!\n");

    //variável para guardar o endereço do cliente
    char cli_ip_addr[16] = {0};

    //convertendo o endereço do cliente para uma forma mais legível
    inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, &cli_ip_addr, sizeof(cli_ip_addr));
    //demonstrando no lado do servidor o endereço do cliente conectado
    printf("Endereço do cliente conectado: %s\n", cli_ip_addr);

    //depois de criado o socket do servidor e a função accept() aceita conexões de clientes e cria um novo socket (exclusivamente para o cliente) é hora de passar esses clientes para realizarem operações através de threads
    //criando threads e passando funções para elas executarem
    //clisock - é o novo socket criado para o cliente e está sendo passado para a função handle_client() que será executada por threads
    pthread_create(&cli_threads[clients], 0, handle_client, (void*)clisock);
    clients++;//incremento para acessar a próxima thread cliente
  }

  return(0);
}