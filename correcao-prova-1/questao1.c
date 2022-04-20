#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define QTD_LETRAS 26
#define DIC_NAME 10

void* gera_dic_thread(void* p){
    return(0);
}

int gera_dic_proc(char c){
    return(0);
}

int main(void){
    char opcao;
    pthread_t threads[QTD_LETRAS];
    int child_pids[QTD_LETRAS];
    int pid_original;

    pid_original = getpid();

    printf("Processo[p] ou Thread[t]: ");
    scanf("%c", opcao);

    //se for threads
    if(opcao == "t"){
        //execução com threads
        for(long i = 0; i < QTD_LETRAS; i++){
            pthread_create(&threads[i], NULL, gera_dic_thread, (void*)i);
        }

        for(int i = 0; i < QTD_LETRAS; i++){
            pthread_join(threads[i], NULL);
        }
        
        char letra = "a";
        for(int i = 0; i < QTD_LETRAS; i++){
            char filename[10];
            FILE* textfile;
            FILE* dicFile;
            sprintf(filename, "%c.txt", letra + 1);
            //lendo filename
            textfile = fopen(filename, "f");
            dicFile = fopen(DIC_NAME, "w");
            //ler de um arquivo e escreve em outro
        }

    } else if(opcao == "p"){ //se for processos
        //execução com processos (bloco executa 26 vezes)
        for(int i = 0; i < QTD_LETRAS; i++){
            if(getpid() == pid_original){ //processo original
                int fork_return;
                fork_return = fork();
                if(fork_return == 0){
                    //processo clonado
                    char c = "a";
                    gera_dic_proc(c + i);
                    exit(0);
                } else {
                    //processo original
                    //waitpid(fork_return, NULL, NULL);
                    //armazenar o pid do processo clonado no array para clonar de novo, não espera os outros processos terminarem a execução
                    child_pids[i] = fork_return;
                }
            }

            //espera todos os clonados acabarem
            for(int i = 0; i < QTD_LETRAS; i++){
                waitpid(child_pids[i], NULL, NULL);
            }

            //junta todos os arquivos em 1
        }
    } else {
        printf("Opção inválida!\n");
        exit(0);
    }



    return(0);
}