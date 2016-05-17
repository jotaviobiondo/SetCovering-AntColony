#include "aco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "util.h"

instancia_t instancia;

/* PARÂMETROS */
double alfa;
double beta;
double rho;
double q0;       // Ferômonio inicial

int n_formigas;
int n_ciclos;

formiga_t *lista_formigas;
double *feromonio;

void inicializarFormiga(formiga_t *formiga){
    formiga->colunas = lista_criar();
    formiga->qtd_colunaCobreLinha = (int*)(calloc(instancia.l, sizeof(int)));
}

void inicializarVariaveis(){
    lista_formigas = (formiga_t*)(malloc(n_formigas * sizeof(formiga_t)));
    feromonio = (double*)(malloc(instancia.c * sizeof(double)));

    int i;
    for (i = 0; i < n_formigas; ++i){
        inicializarFormiga(&lista_formigas[i]);
    }
}

void inicializarFeromonio(){
    int i;
    for (i = 0; i < instancia.c; i++){
        feromonio[i] = q0;
    }
}

void ant_colony(){
    inicializarVariaveis();
    inicializarFeromonio();

    int i;
    for (i = 0; i < n_ciclos; ++i){
        //construirSolucoesFormigas();
        //atualizarFeromonio();
    }
}
