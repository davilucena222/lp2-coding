#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void){
    int pid_1, pid_2;
    int pid;
    int valor;
    int status_1, status_2;

    printf("processo original: %d\n", getpid());

    //retorno da função fork informa em qual instância de processo está sendo executado
    pid_1 = fork();

    if(pid_1 == 0){
        //processo clonado
        printf("processo clonado: %d\n", getpid());
        printf("digite um número inteiro (1): ");
        scanf("%d", &valor);
        printf("primeiro clone saindo com o código %d\n", valor);
        exit(valor);

        //sem o EXIT_SUCCESS o processo clonado continuaria a ser executado
        // exit(EXIT_SUCCESS); //retorna 0
    } else {
        //processo original
        pid_2 = fork();
        if(pid_2 == 0){
            //segundo clone
            printf("segundo clone: %d\n", getpid());
            // sleep(30);
            printf("digite um número inteiro (2): \n");
            scanf("%d", &valor);
            printf("segundo clone finalizando %d... \n", valor);
            exit(111);
        }
    }

    printf("processo original aguardando finalização do pid: %d\n", pid_1);
    waitpid(-1, &status_1, 0); 
    //printf("processo original aguardando finalização do pid: %d\n", pid_2);
    //waitpid(pid_2, &status_2, 0); 

    printf("processo original: %d\n", getpid());
    printf("processo %d finalizado com sucesso! status: %d\n", pid_1, WEXITSTATUS(status_1));
    //printf("processo %d finalizado com sucesso! status: %d\n", pid_2, WEXITSTATUS(status_2));


    return 0;
}