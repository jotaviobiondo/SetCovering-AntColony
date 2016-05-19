#ifndef LISTA_H
#define LISTA_H

struct Lista {
    int *elem;
    int tamMax;
    int tam;
};

typedef struct Lista lista_t;

lista_t* lista_criar();
lista_t* lista_criarTam(int tam);
void lista_insere(lista_t *lista, int elem);
int lista_remove(lista_t *lista, int pos);
int lista_obter(lista_t *lista, int pos);
int lista_vazia(lista_t *lista);
void lista_free(lista_t *lista);

#endif
