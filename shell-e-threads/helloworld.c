#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h> //p - poxix (familia de SO's que dão suporte a funções de threads)

void* printHello(void *p) {
    printf("Hello World!\n");
    while(1); //bloqueia a execução do programa
    return 0;
}

//thread é uma linha de execução
//primeira thread do programa
//função main() é escalonada
//todo processo tem pelo menos uma threadt
int main(void) {
    //quando a main para de executar todas as outras threads também param de executar

    //armazena referências para threads 
    pthread_t t1, t2;

    //cria thread e retorna um inteiro
    //threads que são executadas de forma concorrente junto com a função main
    pthread_create(&t1, NULL, printHello, NULL);
    pthread_create(&t2, NULL, printHello, NULL);

    printf("Hello\n");
    sleep(10);
    //while(1);
    //printf("Hello?\n");

    return(0);
}