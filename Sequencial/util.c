#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void erroSair(char *msg){
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}

double random_double(){
    double out;
    out = (double)rand()/(RAND_MAX + 1);
    out = (rand() + out)/RAND_MAX;
    out = (rand() + out)/RAND_MAX;
    out = (rand() + out)/RAND_MAX;
    out = (rand() + out)/RAND_MAX;
    out = (rand() + out)/RAND_MAX;
    
    return out;
}

int random_int(int limite){
    int rand_int = rand() % limite;
    return rand_int;
}

int tam_intersecao(int *vet1, int *vet2, int tam1, int tam2){
    int i, j, cont = 0;
    for (i = 0; i < tam1; i++){
        int elem = vet1[i];
        for (j = 0; j < tam2; j++){
            if (vet2[j] == elem){
                cont++;
            }
        }
    }
    return cont;
}
