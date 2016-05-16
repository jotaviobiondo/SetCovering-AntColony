#include "aco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void inicializarParametros(){
    lista_formigas = (formiga_t*)(malloc(n_formigas * sizeof(formiga_t)));
    feromonio = (double*)(malloc(instancia.c * sizeof(double)));

    alfa = 1.0;
    beta = 2.0;
    rho = 0.1;
    q0 = 1.0;    
    n_formigas = 10;
    n_ciclos = 100;
}

void inicializarFeromonio(){
    int i;
    for (i = 0; i < instancia.c; i++){
        feromonio[i] = q0;
    }
}

void ant_colony(){
    inicializarParametros();
    inicializarFeromonio();

    int i;
    for (i = 0; i < n_ciclos; ++i){
        //construirSolucoesFormigas();
        //atualizarFeromonio();
    }
}
