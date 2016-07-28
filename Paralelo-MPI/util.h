#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

#define VERBOSE(STR, ...) if (verbose) printf(STR, ## __VA_ARGS__)

extern bool verbose;

void erroSair(char *msg);
int random_int(int limite);
int tam_intersecao(int *vet1, int *vet2, int tam1, int tam2);

#endif
