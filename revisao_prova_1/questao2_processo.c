#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define QTD_ARQUIVOS 6

//programa para buscar strings fornecidas pelo usuário dentro de arquivos fazendo uso de um processo em cada execução de busca

//função que realiza a busca da string
//primeiro parâmetro é a string que vai ser buscada e o segundo parâmetro é em qual arquivo/posição do arquivo tal string vai ser buscada
int busca_string(char* string, char* arquivo){
    sleep(2);

    // gerando números aleatórios de 0 à 100 para ser o valor de retorno da função busca_string()
    int return_value = rand() % 100;
    printf("Encontrei %d instâncias da string %s no arquivo %s.txt\n", return_value, string, arquivo);
    
    return return_value;
}

int main(void) {
    //variável para capturar o pid do processo original
    int pid_original;

    //variável para guardar o valor de retorno que é o pid dos processos clonados fornecido pela execução da função fork()
    int pid_clones[QTD_ARQUIVOS];

    //variável que vai somar o total das execuções dos processos clonados (soma os pid's dos processos clonados que foram criados)
    int return_clones[QTD_ARQUIVOS];

    //variável que guarda a string fornecida pelo usuário
    char string_busca[9];

    //variável para guardar as strings dentro de um arquivo
    char filename[16];

    //variável para gerar valores aleatórios
    time_t tempo;
    srand(time(&tempo));

    //solicitando ao usuário a string
    printf("Digite a string a buscar: ");
    fgets(string_busca, sizeof(string_busca), stdin);

    //capturando o pid do processo original
    pid_original = getpid();

    //bloco for para criar os 6 processos que serão utilizados para buscar as strings nos arquivos
    //clonando processos
    for(int i = 0; i < QTD_ARQUIVOS; i++){
        if(pid_original == getpid()){
            //clonando processos dentro do processo original e guardando o pid dos processos clonados que foram retornados pela execução da função fork()
            pid_clones[i] = fork();
        }
    }

    //verificando se está dentro do escopo do processo clonado para realizar operações de busca da string informada pelo usuário
    //processos clonados utilizados para formatar a string e guardar dentro de um arquivo (primeira posição do array do arquivo) e retornar o valor de busca (que vem da função busca_string()) da string que está sendo procurada (um valor aleatório entre 0 e 100)
    if(pid_original != getpid()){
        //formatando a string e guardando ela dentro da variável array filename
        //terceiro parâmetro é referente a posição do array filename[]
        //inferindo o nome do arquivo pelo pid do processo clonado
        sprintf(filename, "%d", getpid() - pid_original);

        printf("Processo buscando no arquivo %d.txt\n", getpid() - pid_original);

        //retorna a string que está sendo buscada pelo processo
        return busca_string(string_busca, filename);
    }

    //verificando se está dentro do escopo do processo original para demonstra a soma total de ocorrências encontradas
    //processo original utilizado para somar o total de ocorrências (valor de retorno do pid do processo clonado)
    if(pid_original == getpid()){
        //variável para guardar a soma total de ocorrências 
        int soma_ocorrencias = 0;

        //bloco for para realizar a soma total dos valores de retorno da função fork() referente ao pid do processo clonado
        for(int i = 0; i < QTD_ARQUIVOS; i++){
            //esperando o valor de retorno do pid do processo clonado através da função waitpid() e repassando para a variável "return_clone"
            waitpid(pid_clones[i], &return_clones[i], 0);

            //somando cada valor de retorno capturado para obter o total
            soma_ocorrencias = soma_ocorrencias + WEXITSTATUS(return_clones[i]);
        }

        printf("Quantidade de ocorrências total para %s: %d\n", string_busca, soma_ocorrencias);
    }

    return(0);
}