#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void){

    int pid;
    int valor;
    int status;

    printf("processo original: %d\n", getpid());

    //retorno da função fork informa em qual instância de processo está sendo executado
    pid = fork();

    if(pid == 0){
        //processo clonado
        printf("processo clonado: %d\n", getpid());
        printf("digite um número inteiro: ");
        scanf("%d", &valor);
        exit(valor);

        //sem o EXIT_SUCCESS o processo clonado continuaria a ser executado
        // exit(EXIT_SUCCESS); //retorna 0
    } 

    printf("processo original aguardando finalização do pid: %d\n", pid);
    //processo original
    //(valor de pid, endereço de status para armazenar o valor de saída do outro processo, e 0 porque não tem nenhuma opção extra)
    //processo original não vai progredir até o processo clonado ser finalizado
    waitpid(pid, &status, 0); 

    printf("processo original: %d\n", getpid());
    printf("processo %d finalizado com sucesso! status: %d\n", pid, WEXITSTATUS(status));


    return 0;
}