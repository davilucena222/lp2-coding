#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <semaphore.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFF_SIZE 32

typedef struct monitor {
  char palavra[BUFF_SIZE];
  sem_t mutex;
} t_monitor;

void mon_get_palavra(t_monitor* t) {
  printf("Palavra Monitor: %s", t -> palavra);
}

void mon_altera_palavra(t_monitor *t, char n_palavra[]){
  sem_wait(&t -> mutex);
  strcpy(t -> palavra, n_palavra);
  sem_post(&t -> mutex);
}

void handle_client(int cli_sock, t_monitor *t){
  char buffer[BUFF_SIZE + 1];

  while(1){
    read(cli_sock, buffer, BUFF_SIZE);
    // buffer[BUFF_SIZE] = '\0';
    printf("Recebi do servidor: %s\n", buffer);

    if(strncmp("exit", buffer, 4) == 0){
      break;
    }

    mon_altera_palavra(t, buffer);

    mon_get_palavra(t);

    char str_return[BUFF_SIZE];
    sprintf(str_return, "%s\n", buffer);
    write(cli_sock, str_return, sizeof(str_return));
  }
}

int main(void){
  t_monitor monitor;

  int server_socket;
  int cli_socket;
  int server_port;

  int bind_result;
  int listen_result;

  int cli_addr_size;
  struct sockaddr_in server_addr;
  struct sockaddr_in cli_addr;

  sem_init(&monitor, 0, 1);

  printf("Digite a porta do servidor: ");
  scanf("%d", &server_port);

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if(server_socket < 0){
    perror("Socket() error!\n");
    exit(-1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(server_port);

  bind_result = bind(server_socket, (struct sockaddr_in*)&server_addr, sizeof(server_addr));

  if(bind_result < 0){
    perror("Error - bind()\n");
    exit(-1);
  }

  printf("Aguardando conexões na porta %d\n", server_port);

  listen_result = listen(server_socket, 1);

  if(listen_result < 0){
    perror("Error - listen()\n");
    exit(-1);
  }

  while(1){
    int cli_sock = accept(server_socket, (struct sockaddr_in*)&cli_addr, &cli_addr_size);

    if(cli_sock < 0){
      perror("Error - accept()\n");
      exit(-1);
    }

    handle_client(cli_sock, &monitor);
    break;
  }

  return(0);
}