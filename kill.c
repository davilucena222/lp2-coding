#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(void){
    int pid_clonado, retorno_kill;

    pid_clonado = fork();

    //enquanto o processo original existir por cerca de 5 segundos, o processador clonado estará sendo executado e exibindo as mensagens
    if(!(pid_clonado == 0)){
        //processo original
        printf("pid: %d, ppid: %d\n", getpid(), getppid());
        sleep(5);
        //primeiro parâmetro é o PID, e o segundo parâmetro são comandos de dentro do próprio kill()
        retorno_kill = kill(pid_clonado + 100, SIGHUP);
        printf("retorno kill: %d\n", retorno_kill);
    } else {
        //processo clonado
        printf("pid: %d, ppid: %d\n", getpid(), getppid());
        while(1)
            printf("flood\n");
    }

    printf("fim!\n");

    return(0);
}