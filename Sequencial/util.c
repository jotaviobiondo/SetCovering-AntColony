#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void erroSair(char *msg){
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}