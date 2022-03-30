#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
    int n, fork_return, pid_original;

    printf("antes dos fork\n");

    //pid presente em processos clonados
    pid_original = getpid();

    printf("digite n: \n" );
    scanf("%d", &n);
    n--; //decremento para criar processos exatos ao número que o usuário forneceu

    //fork_return = fork();

    for(int i = 0; i < n; i++){
       if(pid_original == getpid()){
            printf("%d\n", pid_original);
            //processo original
            fork();
        } else {
            //processo clonado   
        }
    }    
    
    printf("meu pid: %d, meu ppid: %d\n", getpid(), getppid()); 
    printf("depois do fork\n");
    //sleep(30);
    while(1);


    return 0;
}

//comando kill() só finaliza os programas (processos) que o programador criou, para finalizar outros programas é precisor ter permissão de administrador
//