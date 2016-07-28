#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

#define INI_TAM 40

lista_t* lista_criar(){
    lista_t *lista = (lista_t*) malloc(sizeof(lista_t));
    lista->elem = (int*) malloc(INI_TAM * sizeof(int));
    lista->tamMax = INI_TAM;
    lista->tam = 0;
    return lista;
}

lista_t* lista_criarTam(int tam){
    lista_t *lista = (lista_t*) malloc(sizeof(lista_t));
    lista->elem = (int*) malloc(tam * sizeof(int));
    lista->tamMax = tam;
    lista->tam = 0;
    return lista;
}

void crescerLista(lista_t *lista){
    lista->tamMax *= 2;
    int *ptr = (int*) realloc(lista->elem, lista->tamMax * sizeof(int));
    if (ptr == NULL){
        printf("ERRO: Falha no realloc.\n");
    }
    lista->elem = ptr;
}

void lista_insere(lista_t *lista, int elem){
    lista->elem[lista->tam] = elem;
    lista->tam++;
    if (lista->tam == lista->tamMax){
        crescerLista(lista);
    }
}

int lista_remove(lista_t *lista, int pos){
    if (pos >= lista->tam){
        printf("ERRO: Posicao invalida para a lista.\n");
        return -1;
    }

    int ret = lista->elem[pos];

    int i;
    for (i = pos; i < lista->tam - 1; i++){
        lista->elem[i] = lista->elem[i+1];
    }
    lista->tam--;

    return ret;
}

int lista_removeElem(lista_t *lista, int elem){
    int i;
    for (i = 0; i < lista->tam; i++){
        if (lista->elem[i] == elem){
            lista_remove(lista, i);
            return i;
        }
    }

    return -1;
}

int lista_obter(lista_t *lista, int pos){
    return lista->elem[pos];
}

int lista_contem(lista_t *lista, int elem){
    int i;
    for (i = 0; i < lista->tam; i++){
        if (lista->elem[i] == elem){
            return 1;
        }
    }
    return 0;
}

int lista_vazia(lista_t *lista){
    return lista->tam == 0;
}

void lista_free(lista_t *lista){
    free(lista->elem);
    free(lista);
}

void lista_resetar(lista_t *lista){
    lista->tam = 0;
}

