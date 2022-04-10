#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define COMMAND_SIZE 16

int main(void) {
    int fork_result;
    int status_exec;

    char *command = (char*) malloc(sizeof(char)*COMMAND_SIZE);
    char* clear = (char*) malloc(sizeof(char)*COMMAND_SIZE);

    do {
        char* lista_comm[2];

        if(command == NULL){
            command = (char*) malloc(sizeof(char)*COMMAND_SIZE);
        }
        if(clear == NULL){
            clear = (char*) malloc(sizeof(char)*COMMAND_SIZE);
        }

        printf("Comando: ");
        fgets(command, COMMAND_SIZE, stdin);

        //command recebido sem o último caractere "\n"
        strncpy(clear, command, strlen(command)-1);

        lista_comm[0] = clear;
        lista_comm[1] = NULL; //sinaliza a quantidade de strings armazenadas no array, determina o ponto final do array

        fork_result = fork();
        if(fork_result == 0){
            //processo clonado
            printf("Executando: %s\n", clear);
            //ignora o que estiver abaixo, e só o processo original passa a existir
            execvp(lista_comm[0], lista_comm);
        } else {
            //processo original
            wait(&status_exec);
            printf("Status recebido: %d\n", status_exec);
        }
        if(command != NULL){
            free(command);
        }

        if(clear != NULL){
            free(clear);
        }

    } while(strcmp(command, "exit\n") != 0); //executa enquanto o comando recebido não for "exit"

    if(fork_result > 0){
        printf("Saindo... \n");
    }
    
    //char *argv[] = {"ls", "-l", "-h", "-a", NULL};

    //função que executa um processo externo (mas antes faz uma chamada ao SO)
    //execvp(argv[0], argv);

    return(0);
}