#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(void){
    int pid_original;

    pid_original = getpid();

    fork();
    fork();

    //enquanto o processo original existir por cerca de 5 segundos, o processador clonado estará sendo executado e exibindo as mensagens
    if(pid_original == getpid()){
        sleep(10);
        //primeiro parâmetro é o PID, e o segundo parâmetro são comandos de dentro do próprio kill()
        //passando zero como parâmetro mata o grupo de processos
        //passando -1 finaliza todos os PID'S anteriores ao meu PID (PID desse programa)
        kill(0, SIGHUP);
        printf("finalizei grupo!");
    } else {
        //processo clonado
        printf("pid: %d, ppid: %d\n", getpid(), getppid());
        while(1);
    }

    printf("fim!\n");

    return(0);
}