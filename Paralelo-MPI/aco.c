#include "aco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include "lista.h"
#include "util.h"

#define INT_MAX 2147483647

struct DeltaFeromonio {
    int coluna;
    double delta_feromonio;     // Variação do feromônio 
};

typedef struct DeltaFeromonio delta_feromonio_t;

MPI_Datatype MPI_FEROMONIO_TYPE;

delta_feromonio_t *lista_delta_feromonio;
int num_delta_feromonio;

instancia_t instancia;

/* PARÂMETROS */
double alfa;    // influência no feromônio
double beta;    // influência na heurística
double rho;     // taxa de evaporação
double q0;      // ferômonio inicial

int n_formigas;
int n_ciclos;

int n_processo;

int formiga_por_processo;

formiga_t *lista_formigas;
formiga_t melhor_formiga;            // Melhor formiga do processo
int custo_melhor_formiga;
double *feromonio;                   // vetor do feromonio depositado em cada coluna


/* Funções para lista de delta feromônio */

void inserirListaDeltaFeromonio(int coluna, double delta){
    int i;
    for (i = 0; i < num_delta_feromonio; i++){
        if (lista_delta_feromonio[i].coluna == coluna){
            lista_delta_feromonio[i].delta_feromonio += delta;
            return;
        }
    }
    lista_delta_feromonio[num_delta_feromonio].coluna = coluna;
    lista_delta_feromonio[num_delta_feromonio].delta_feromonio = delta;
    num_delta_feromonio++;
}

void resetarListaDeltaFeromonio(){
    memset(lista_delta_feromonio, 0, num_delta_feromonio*sizeof(delta_feromonio_t));
    num_delta_feromonio = 0;
}

/* Funções para a formiga */

void inicializarFormiga(formiga_t *formiga){
    formiga->colunas = lista_criar();
    formiga->qtd_colunaCobreLinha = (int*)(calloc(instancia.l, sizeof(int)));
    formiga->custo_total = 0;
}

void resetarFormiga(formiga_t *formiga){
    lista_resetar(formiga->colunas);
    memset(formiga->qtd_colunaCobreLinha, 0, instancia.l * sizeof(int));
    formiga->custo_total = 0;
}

void inicializarVariaveis(){
    int i;
    for (i = 0; i < formiga_por_processo; ++i){
        inicializarFormiga(&lista_formigas[i]);
    }
}

double heuristica(int coluna, lista_t *linhasDescobertas){
    int tam_intersec = tam_intersecao(instancia.coluna[coluna], linhasDescobertas->elem, instancia.nlinhas[coluna], linhasDescobertas->tam);

    double visibilidade = (double) tam_intersec / instancia.custo[coluna];
    return visibilidade;
}

double calcProbNumerador(int coluna, formiga_t *formiga, lista_t *linhasDescobertas){
    if (lista_contem(formiga->colunas, coluna)){
        return 0.0;
    }

    double h = heuristica(coluna, linhasDescobertas);
    double numerador = pow(feromonio[coluna], alfa) * pow(h, beta);
    return numerador;
}

double calcProbDenominador(formiga_t *formiga, lista_t *linhasDescobertas){
    int i;
    double denominador = 0.0, h;
    for (i = 0; i < instancia.c; i++){
        if (!lista_contem(formiga->colunas, i)){
            h = heuristica(i, linhasDescobertas);
            denominador += pow(feromonio[i], alfa) * pow(h, beta);
        }
    }
    return denominador;
}

int maximizarProbabilidade(int linha, lista_t *linhasDescobertas, formiga_t *formiga){
    int i, coluna;
    double probabilidade, numerador, denominador;
    double maior = 0.0;
    int melhorColuna = -1;
    denominador = calcProbDenominador(formiga, linhasDescobertas);
    for (i = 0; i < instancia.ncolunas[linha]; i++){
        coluna = instancia.linha[linha][i];

        numerador = calcProbNumerador(coluna, formiga, linhasDescobertas);
        probabilidade = numerador / denominador;

        if (probabilidade > maior){
            maior = probabilidade;
            melhorColuna = coluna;
        }
    }

    return melhorColuna;
}

void addColuna(formiga_t *formiga, int coluna, lista_t *linhasDescobertas){
    lista_insere(formiga->colunas, coluna);
    formiga->custo_total += instancia.custo[coluna];
    
    int i;
    for (i = 0; i < instancia.nlinhas[coluna]; i++){
        int linha = instancia.coluna[coluna][i];
        formiga->qtd_colunaCobreLinha[linha]++;
        lista_removeElem(linhasDescobertas, linha);
    }
}

void removeColuna(formiga_t *formiga, int coluna){
    lista_removeElem(formiga->colunas, coluna);
    formiga->custo_total = formiga->custo_total - instancia.custo[coluna];

    int i;
    for (i = 0; i < instancia.nlinhas[coluna]; i++){
        formiga->qtd_colunaCobreLinha[instancia.coluna[coluna][i]]--;
    }
}

void construirSolucao(formiga_t *formiga){
    int i;
    lista_t *linhasDescobertas = lista_criarTam(instancia.l);
    for (i = 0; i < instancia.l; i++){
        lista_insere(linhasDescobertas, i);
    }

    while (!lista_vazia(linhasDescobertas)){
        int rand_int = random_int(linhasDescobertas->tam);
        int linha = lista_obter(linhasDescobertas, rand_int);

        int melhorColuna = maximizarProbabilidade(linha, linhasDescobertas, formiga);
        addColuna(formiga, melhorColuna, linhasDescobertas);
    }
}

int colunaRedundante(formiga_t *formiga, int coluna){
    int i, linha;
    for (i = 0; i < instancia.nlinhas[coluna]; i++){
        linha = instancia.coluna[coluna][i];
        if (formiga->qtd_colunaCobreLinha[linha] < 2){
            return 0;
        }
    }
    return 1;
}

void eliminarRedundancia(formiga_t *formiga){
    lista_t *T = lista_criarTam(formiga->colunas->tam);
    int i;
    for (i = 0; i < formiga->colunas->tam; i++){
        lista_insere(T, formiga->colunas->elem[i]);
    }

    for (i = T->tam - 1; i >= 0; i--){
        int coluna = T->elem[i];
        lista_remove(T, i);
        if (colunaRedundante(formiga, coluna)){
            removeColuna(formiga, coluna);
        }
    }
}

/* Funções do Ant Colony Optimization */

void construirSolucoesFormigas(){
    int i;
    for (i = 0; i < formiga_por_processo; ++i){
        construirSolucao(&lista_formigas[i]);
        eliminarRedundancia(&lista_formigas[i]);

        if (lista_formigas[i].custo_total < melhor_formiga.custo_total){
            melhor_formiga = lista_formigas[i];
        }
    }
}

void resetarFormigas(){
    int i;
    for (i = 0; i < formiga_por_processo; i++){
        resetarFormiga(&lista_formigas[i]);
    }
}

void evaporarFeromonio(int procID){
    int i;
    int bloco = instancia.c / n_processo;
    int inicio = procID * bloco;
    int fim;
    if (procID != n_processo - 1){
        fim = inicio + bloco;
    } else { // se for o ultimo processo
        fim = instancia.c;
    }

    for (i = inicio; i < fim; i++){
        inserirListaDeltaFeromonio(i, -rho * feromonio[i]);
        //feromonio[i] = (1 - rho) * feromonio[i];
    }
}

void depositarFeromonio(){
    int i, j;
    for (i = 0; i < formiga_por_processo; i++){
        for (j = 0; j < lista_formigas[i].colunas->tam; j++){
            int coluna = lista_formigas[i].colunas->elem[j];
            double deposito = ((double)1.0 / lista_formigas[i].custo_total);
            //feromonio[coluna] = feromonio[coluna] + deposito;
            inserirListaDeltaFeromonio(coluna, deposito);
        }
    }
}

void atualizarFeromonio(int procID){
    evaporarFeromonio(procID);
    depositarFeromonio();
}

void receberDeltaFeromonio(int id){
    int i, total_recebido = 0;
    int num_recebidos[n_processo];    // vetor de número de elementos recebidos
    int displ[n_processo];            // vetor de deslocamentos para o allgatherv
    delta_feromonio_t rcv_buffer[instancia.c*n_processo]; // buffer que receberá o delta feromonio
    
    MPI_Allgather(&num_delta_feromonio, 1, MPI_INT, num_recebidos, 1, MPI_INT, MPI_COMM_WORLD);
    
    displ[0] = 0;
    for (i = 1; i < n_processo; i++) {
        displ[i] = displ[i-1] + num_recebidos[i-1];
    }
    total_recebido = displ[n_processo-1] + num_recebidos[n_processo-1];

    MPI_Allgatherv(lista_delta_feromonio, num_delta_feromonio, MPI_FEROMONIO_TYPE, 
                   rcv_buffer, num_recebidos, displ, MPI_FEROMONIO_TYPE, MPI_COMM_WORLD);

    for (i = 0; i < total_recebido; i++){
        feromonio[rcv_buffer[i].coluna] += rcv_buffer[i].delta_feromonio;
    }
}

void ant_colony(int procID){
    inicializarVariaveis();

    const int nitems = 2;
    int tam_blocos[2] = {1, 1};
    MPI_Datatype tipos[2] = {MPI_INT, MPI_DOUBLE};
    MPI_Aint offsets[2];

    offsets[0] = offsetof(delta_feromonio_t, coluna);
    offsets[1] = offsetof(delta_feromonio_t, delta_feromonio);

    MPI_Type_create_struct(nitems, tam_blocos, offsets, tipos, &MPI_FEROMONIO_TYPE);
    MPI_Type_commit(&MPI_FEROMONIO_TYPE);

    int i;
    for (i = 0; i < n_ciclos; ++i){
        construirSolucoesFormigas();
        atualizarFeromonio(procID);

        receberDeltaFeromonio(procID);

        resetarFormigas();
        resetarListaDeltaFeromonio();

        VERBOSE("Ciclo: %d\n", i);
        VERBOSE("Melhor Formiga para o processo %d: %d\n", procID, melhor_formiga.custo_total);
        VERBOSE("-------------------------\n");
    }

    MPI_Reduce(&melhor_formiga.custo_total, &custo_melhor_formiga, 1, 
               MPI_INT, MPI_MIN, PROC_ROOT, MPI_COMM_WORLD);

    MPI_Type_free(&MPI_FEROMONIO_TYPE);
}

void inicializar_parametros(){
    alfa = 0.4;
    beta = 0.8;
    rho = 0.4;
    q0 = 1.0;
    n_formigas = 96;
    n_ciclos = 15;
}

void inicializarFeromonio(){
    int i;
    for (i = 0; i < instancia.c; i++){
        feromonio[i] = q0;
    }
}

void inicializar_aco(){
    formiga_por_processo = n_formigas / n_processo;

    lista_formigas = (formiga_t*)(malloc(formiga_por_processo * sizeof(formiga_t)));
    melhor_formiga.custo_total = INT_MAX;
    feromonio = (double*)(malloc(instancia.c * sizeof(double)));
    lista_delta_feromonio = (delta_feromonio_t*)(calloc(instancia.c, sizeof(delta_feromonio_t)));
    num_delta_feromonio = 0;

    inicializarFeromonio();
}

void free_aco(){
    free(lista_formigas);
    free(feromonio);
}
