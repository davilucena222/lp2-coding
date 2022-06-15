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
#include <netdb.h> //fazer uso do DNS
#include <netinet/in.h>

#define QTD_CLIENTS 5
#define BUFF_SIZE 16
#define PORT 9999

typedef struct monitor {
    int soma;
    sem_t mutex;
} t_monitor;

t_monitor monitor_soma;

int mon_get_soma(void){
    return monitor_soma.soma;
}

void mon_inc_soma(int n){
    sem_wait(&monitor_soma.mutex); //protocolo de entrada
    monitor_soma.soma += n;
    sem_post(&monitor_soma.mutex); //protocolo de saída
}


void* handle_client(void* p){
    int cli_sock = (int)p; //recebe o socket (instâncias diferentes)

    //buffer para se comunicar com o cliente
    char buffer[BUFF_SIZE + 1];

    //realizando a operação
    while(1){
        //lendo a informação do cliente passando o tipo de socket exclusivo
        read(cli_sock, buffer, BUFF_SIZE);
        buffer[BUFF_SIZE] = '\0'; //ponto final da string
        printf("Recebi: %s\n", buffer);

        //convertendo o buffer para um número inteiro
        int num = atoi(buffer);

        //se o cliente enviar 0 para o servidor, o servidor será finalizadp
        if(num == 0){
            printf("Finalizando servidor...\n");
            exit(0);
        }

        //somando no monoitor
        //incrementando a soma
        mon_inc_soma(num);

        //recuperando o valor passado antes de duplicar pela função mon_get_soma()
        printf("Soma: %d\n", mon_get_soma());

        //duplica o valor recebido
        num *= 2;

        //armazenando as informações passadas
        char str_return[BUFF_SIZE];
        sprintf(str_return, "%d\n", num); //convertendo string para inteiro
        printf("Retornei: %d\n", num);

        //enviando o valor duplicado de volta ao cliente
        write(cli_sock, str_return, strlen(str_return));
    }
}

int main(void){
    int server_sock;
    int cli_sock[QTD_CLIENTS];
    pthread_t cli_threads[QTD_CLIENTS];

    int clients = 0;

    struct sockaddr_in server_addr;
    struct sockaddr_in cli_addr;
    int cli_addr_size;

    sem_init(&monitor_soma.mutex, 0, 1);

    printf("Iniciando servidor de soma na porta %d...!\n", PORT);

    server_sock = socket(AF_INET, SOCK_STREAM, 0); //servidor TCP (garante e conexão)

    if(server_sock < 0){
        perror("socket() error!\n");
        exit(-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //sinalizado ao sistema operacional que está criando um servidor
    server_addr.sin_port = htons(PORT);

    //associando a estrutura criada ao socket
    int bind_result = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(bind_result < 0){
        perror("Bind() error!\n");
        exit(-1);
    }

    //passando os clientes para o servidor
    int listen_result = listen(server_sock, QTD_CLIENTS);

    if(listen_result < 0){
        perror("listen() error!\n");
        exit(-1);
    }

    //loop infinito para aguardar as conexões dos clientes
    while(1){
        //função bloqueante que aguarda as conexões do cliente
        int clisock = accept(server_sock, (struct sockaddr*)&cli_addr, &cli_addr_size);

        //passou do accept() é criado um novo socket
        if(clisock < 0){
            perror("accept() error!\n");
            exit(-1);
        }

        printf("Novo cliente conectado!\n");

        //informações do cliente
        char cli_ip_addr[16] = {0};

        //convertendo o endereço do cliente para uma forma mais legível
        inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, cli_ip_addr, sizeof(cli_ip_addr));
        printf("Endereço do cliente: %s\n", cli_ip_addr);

        //socket passada para a thread tratar a operação
        pthread_create(&cli_threads[clients], 0, handle_client, (void*)clisock);
        clients++;
    }


    return(0);
}