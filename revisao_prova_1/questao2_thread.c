#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define QTD_ARQUIVOS 6

//variável global para guardar a string fornecida pelo usuário
char string_busca[9];

//função para buscar a string dentro dos arquivos
long busca_string(char* string, char* arquivo) {
    sleep(2);

    //gerando valor aleatório para o retorno da função
    long return_value = rand() % 100;
    printf("Encontrei %ld instâncias da string %s no arquivo %s.txt\n", return_value, string, arquivo);

    return return_value;
}

//função para a thread executar 
//a string fornecida pelo usuário vai ser armazenada através dessa função
void* busca_string_thread(void *p) {
    //variável arquivo que vai armazenar a string fornecida pelo usuário
    char _filename[16];

    //variável que vai receber a posição para onde a string será guardada no array
    long indice_do_arquivo = (long)p;

    //formatando a string e guardando ela dentro da variável array filename
    //terceiro parâmetro é referente a posição do array filename[]
    sprintf(_filename, "%ld", indice_do_arquivo);

    //retornando o valor de retorno da função busca_string()
    return(void*) busca_string(string_busca, _filename);
}

int main(void) {
    //variável para armazenar o pid do processo original
    int pid_original;

    //variável para criar as threads que irão executar a busca da string
    pthread_t threads[QTD_ARQUIVOS];

    //variável que armazena o valor de retorno das threads
    int return_thread[QTD_ARQUIVOS];

    //variável que vai armazenar a soma total das ocorrências (basicamente o valor aleatório retornado pela função busca_string()) das threads
    int soma_return_value_threads = 0;

    //variável que vai armazenar a string fornecida pelo usuário em um arquivo
    char filename[16];

    //gerando valores aleatórios
    time_t tempo;
    srand(time(&tempo));

    //solicitando a string ao usuário
    printf("Digite a string a buscar: ");
    fgets(string_busca, sizeof(string_busca), stdin);

    //bloco for para criar as threads
    for(long i = 0; i < QTD_ARQUIVOS; i++) {
        //criando threads
        //a função busca_string_thread() vai ser executada por uma thread
        pthread_create(&threads[i], NULL, busca_string_thread, (void*)i);
    }

    //bloco for para capturar e somar o valor de retorno total das ocorrências das threads
    for(int i = 0; i < QTD_ARQUIVOS; i++) {
        //usando pthread_join() para capturar o valor de retorno de cada thread e passar para a variável "return_thread[i]"
        pthread_join(threads[i], (void*)&return_thread[i]);

        //somando os valores de retorno das threds que foram executadas
        soma_return_value_threads = soma_return_value_threads + return_thread[i];
    }

    printf("Quantidade total encontrada: %d\n", soma_return_value_threads);

    return(0);
}