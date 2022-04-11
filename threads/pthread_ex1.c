#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void* funcao1(void* p){
    printf("Função 1: iniciando\n");
    sleep(1);
    printf("Função 1: 1\n");
    sleep(1);
    printf("Função 1: 2\n");
    sleep(1);
    printf("Função 1: 3\n");
    sleep(1);
    printf("Função 1: 4\n");
    sleep(1);
    printf("Função 1: 5\n");
    sleep(1);
    printf("Função 1: 6\n");
    printf("Função 1: saindo...\n");
}

void funcao2(){
    printf("Função 2: iniciando...\n");
    printf("Função 2: 1\n");
    sleep(1);
    printf("Função 2: 2\n");
    sleep(1);
    printf("Função 2: 3\n");
    sleep(1);
    printf("Função 2: saindo...\n");
}

void funcao3(){
    printf("Função 3: iniciando...\n");
    printf("Função 3: 1\n");
    sleep(1);
    printf("Função 3: 2\n");
    sleep(1);
    printf("Função 3: 3\n");
    sleep(1);
    printf("Função 3: saindo...\n");
}

int main(void) {
    pthread_t t1; //variável armazena a referência para a thread que vai ser criada (variável referencia threads)

    printf("Função main: inicio\n");
    //funcao1(NULL);
    //funcao1(NULL);

    //função 1 sendo executada de forma concorrente após ter sido repassada para uma thread (a execução dela é feita pela thread)
    pthread_create(&t1, NULL, funcao1, NULL);
    funcao2();
    //referência de thread da função 1 (executa a função 1 dentro da funao main, a execução passa a ser em sequência, não existe concorrência após a chamada do pthread_join)
    pthread_join(t1, NULL);
    funcao3();

    printf("Função main: término\n");
    sleep(10);


    return 0;
}