#ifndef ACO_H
#define ACO_H

struct Instancia
{
    int l;          // número de linhas
    int c;          // número de colunas
    int **coluna;   // coluna[i] contém as linhas cobertas pela coluna i
    int **linha;    // linha[i] contém as colunas que cobrem a linha i
    int *ncolunas;  // ncolunas[i] é o número de colunas que cobrem a linha i
    int *nlinhas;   // nlinhas[i] é o número de linhas que são cobertas pela coluna i
    int *custo;     // vetor de custos de cada coluna
};

typedef struct Instancia instancia_t;

extern instancia_t inst;

#endif
