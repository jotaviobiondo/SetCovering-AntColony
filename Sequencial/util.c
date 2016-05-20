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
