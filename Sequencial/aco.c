#include "aco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lista.h"
#include "util.h"

#define INT_MAX 2147483647

instancia_t instancia;

/* PARÂMETROS */
double alfa;    // influência no feromônio
double beta;    // influência na heurística
double rho;     // taxa de evaporação
double q0;      // ferômonio inicial

int n_formigas;
int n_ciclos;

formiga_t *lista_formigas;
formiga_t melhor_formiga;
double *feromonio;          // vetor do feromonio depositado em cada coluna
int somaCustosFormigas;     // soma dos custos de todas as formigas para deposito de feromonio

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
    lista_formigas = (formiga_t*)(malloc(n_formigas * sizeof(formiga_t)));

    int i;
    for (i = 0; i < n_formigas; ++i){
        inicializarFormiga(&lista_formigas[i]);
    }

    melhor_formiga.custo_total = INT_MAX;
    somaCustosFormigas = 0;

    feromonio = (double*)(malloc(instancia.c * sizeof(double)));
}

void freeVariaveis(){
    free(lista_formigas);
    free(feromonio);
}

void inicializarFeromonio(){
    int i;
    for (i = 0; i < instancia.c; i++){
        feromonio[i] = q0;
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

void construirSolucoesFormigas(){
    int i;
    for (i = 0; i < n_formigas; ++i){
        construirSolucao(&lista_formigas[i]);
        eliminarRedundancia(&lista_formigas[i]);
        if (lista_formigas[i].custo_total < melhor_formiga.custo_total){
            melhor_formiga = lista_formigas[i];
        }
        somaCustosFormigas += lista_formigas[i].custo_total;
    }
}

void resetarFormigas(){
    int i;
    for (i = 0; i < n_formigas; i++){
        resetarFormiga(&lista_formigas[i]);
    }

    somaCustosFormigas = 0;
}

void evaporarFeromonio(){
    int i;
    for (i = 0; i < instancia.c; i++){
        feromonio[i] = (1 - rho) * feromonio[i];
    }
}

void depositarFeromonio(){
    int i, j;
    for (i = 0; i < n_formigas; i++){
        for (j = 0; j < lista_formigas[i].colunas->tam; j++){
            int coluna = lista_formigas[i].colunas->elem[j];
            //feromonio[coluna] = feromonio[coluna] + ((double)1.0 / somaCustosFormigas);
            feromonio[coluna] = feromonio[coluna] + ((double)1.0 / lista_formigas[i].custo_total);
        }
    }
}

void atualizarFeromonio(){
    evaporarFeromonio();
    depositarFeromonio();
}

void ant_colony(){
    inicializarVariaveis();
    inicializarFeromonio();

    int i;
    for (i = 0; i < n_ciclos; ++i){
        VERBOSE("Ciclo: %d\n", i);
        construirSolucoesFormigas();
        atualizarFeromonio();
        resetarFormigas();
        VERBOSE("Melhor Formiga: %d\n", melhor_formiga.custo_total);
        VERBOSE("-------------------------\n");
    }

    printf("\nMelhor Formiga: %d\n", melhor_formiga.custo_total);

    freeVariaveis();
}

void inicializar_parametros(){
    alfa = 0.4;
    beta = 0.8;
    rho = 0.4;
    q0 = 1.0;
    n_formigas = 20;
    n_ciclos = 20;
}
