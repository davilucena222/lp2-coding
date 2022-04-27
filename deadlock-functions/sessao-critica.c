#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>

int in1 = 0, in2 = 0;
int var_global;

void* cs1(void* p){
    while(1){
        //<await(!in2) in1 = true;>
        while(__atomic_test_and_set(&in2, 1));
        in1 = 1;
    }
}

void* cs2(void* p){
    while(1){

    }
}

int main(void){
    pthread_t threads[2];

    var_global = 0;

    pthread_create(&threads[0], NULL, cs1, NULL);
    pthread_create(&threads[1], NULL, cs2, NULL);
}