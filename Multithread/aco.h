#ifndef ACO_H
#define ACO_H

#include "lista.h"
#include <pthread.h>

/* STRUCT DA INSTANCIA DO PROBLEMA */
struct Instancia{
    int l;          // número de linhas
    int c;          // número de colunas
    int **coluna;   // coluna[j] contém as linhas cobertas pela coluna j
    int **linha;    // linha[i] contém as colunas que cobrem a linha i
    int *ncolunas;  // ncolunas[i] é o número de colunas que cobrem a linha i
    int *nlinhas;   // nlinhas[j] é o número de linhas que são cobertas pela coluna j
    int *custo;     // vetor de custos de cada coluna
};

typedef struct Instancia instancia_t;

/* STRUCT FORMIGA */
struct Formiga {
    lista_t *colunas;           // colunas que pertencem a solução
    int custo_total;            // custo total da solução
    int *qtd_colunaCobreLinha;  // quantidade de colunas que cobrem a linha i
};

typedef struct Formiga formiga_t;

/* PARÂMETROS */
extern double alfa;     // influência no feromônio
extern double beta;     // influência na heurística
extern double rho;      // taxa de evaporação
extern double q0;       // Ferômonio inicial

extern int n_formigas;
extern int n_ciclos;

extern int n_thread;

extern formiga_t melhor_formiga;

extern instancia_t instancia;

extern pthread_barrier_t b1;

/* FUNÇÔES */

void *ant_colony(void *arg);
void inicializar_parametros();
void inicializar_aco();

#endif
