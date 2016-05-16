#ifndef ACO_H
#define ACO_H

/* STRUCT DA INSTANCIA DO PROBLEMA */
struct Instancia{
    int l;          // número de linhas
    int c;          // número de colunas
    int **coluna;   // coluna[i] contém as linhas cobertas pela coluna i
    int **linha;    // linha[i] contém as colunas que cobrem a linha i
    int *ncolunas;  // ncolunas[i] é o número de colunas que cobrem a linha i
    int *nlinhas;   // nlinhas[i] é o número de linhas que são cobertas pela coluna i
    int *custo;     // vetor de custos de cada coluna
};

typedef struct Instancia instancia_t;

extern instancia_t instancia;

/* STRUCT FORMIGA */
struct Formiga {
    int *colunas;               // colunas que pertencem a solução
    int custo_total;            // custo total da solução
    int *qtd_colunaCobreLinha;  // quantidade de colunas que cobrem a linha i
};

typedef struct Formiga formiga_t;

/* PARÂMETROS */
extern double alfa;
extern double beta;
extern double rho;      // determina quanto do feromônio deve evaporar

extern int n_formigas;
extern int n_ciclos;

/* FUNÇÔES */

void ant_colony();

#endif
