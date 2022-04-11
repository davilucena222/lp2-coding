#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    int inicio;
    int fim;
} t_dois_num;

void* funcao_concorrente(void* p){
    t_dois_num* params = (t_dois_num*)p;
    int start = params->inicio;
    int end = params->fim;
    long i;

    for(i = start; i <= end; i++){
        printf("fc: %ld\n", i);
        sleep(1);
    }

    //return (void*) limit;

    //valor de retorno da função que está sendo executada como thread
    pthread_exit((void*)i); 
}

int funcao_sequencial(int inicio, int fim) {
    int i;
    for(i = inicio; i <= fim; i++){
        printf("fs: %d\n", i);
        sleep(1);
    }

    return i;
}


int main(void) {
    long res_c;
    int res_s;
    pthread_t t1;
    t_dois_num param;

    param.inicio = 1;
    param.fim = 10;
    
    //funcao_concorrente((void*)&param);

    //após a função main terminar todas as threds terminam independente da execução
    pthread_create(&t1, NULL, funcao_concorrente, (void*)&param);
    res_s = funcao_sequencial(0,3);

    //continua a execução da funcao_concorrente() (de forma sequencial) sem concorrência na função main()
    pthread_join(t1, (void**)&res_c);

    printf("main: res_c = %ld\n", res_c); //capturando valor de retorno da funcao_concorrente()
    printf("main: res_s = %d\n", res_s);

    return 0;
}