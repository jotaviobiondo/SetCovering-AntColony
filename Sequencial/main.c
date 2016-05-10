#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aco.h"
#include "util.h"

instancia_t lerArquivo(char *nome){
	int i, j;
	instancia_t inst;

	FILE* arquivo = fopen(nome, "r");

	if (!arquivo){
		erroSair("ERRO: Nao foi possivel abrir o arquivo.");
	}

	if(fscanf(arquivo, "%d %d", &inst.l, &inst.c) != 2){
		erroSair("ERRO: Nao foi possivel ler numero de linhas ou colunas do arquivo.");
	}

	inst.custo = (int*)(malloc(inst.c * sizeof(int)));
	for (i = 0; i < inst.c; ++i){
		if (fscanf(arquivo, "%d", &inst.custo[i]) != 1){
			erroSair("ERRO: Nao foi possivel ler custos.");
		}
	}

	inst.linha = (int**)(malloc(inst.l * sizeof(int*)));
	inst.ncolunas = (int*)(malloc(inst.l * sizeof(int)));
	inst.nlinhas = (int*)(malloc(inst.c * sizeof(int)));
	memset(inst.nlinhas, 0, inst.c * sizeof(int));

	for (i = 0; i < inst.l; ++i){
		if (fscanf(arquivo, "%d", &inst.ncolunas[i]) != 1){
			erroSair("ERRO: Nao foi possivel ler o numero de colunas.");
		}

		inst.linha[i] = (int*)(malloc(inst.ncolunas[i] * sizeof(int)));
		for (j = 0; j < inst.ncolunas[i]; ++j){
			if (fscanf(arquivo, "%d", &inst.linha[i][j]) != 1){
				erroSair("ERRO: Nao foi possivel ler colunas.");
			}
			inst.linha[i][j]--;
			inst.nlinhas[inst.linha[i][j]]++;	// contando numero de linhas que sao cobertas pela coluna
		}
	}

	inst.coluna = (int**)(malloc(inst.c * sizeof(int*)));
	int cont[inst.c];
	for (i = 0; i < inst.c; i++){
		inst.coluna[i] = (int*)(malloc(inst.nlinhas[i]));
		cont[i] = 0;
	}

	for (i = 0; i < inst.l; i++){
		for (j = 0; j < inst.ncolunas[i]; j++){
			int k = inst.linha[i][j];
			inst.coluna[k][cont[k]] = i;
			cont[k]++;
		}
	}

	fclose(arquivo);
	return inst;
}

int main(int argc, char const *argv[]){
	inst = lerArquivo("../Instancias/scp41.txt");
	printf("%d %d\n", inst.l, inst.c);
	/*int i;
	for (i = 1; i <= inst.c; ++i){
		printf("%d ", inst.custo[i-1]);
		if (i % 12 == 0)
			printf("\n");
	}
	int j;
	for (i = 0; i < inst.l; i++){
		printf("%d\n", inst.ncolunas[i]);
		for (j = 0; j < inst.ncolunas[i]; j++){
			printf("%d ", inst.linha[i][j]);
		}
		printf("\n");
	}
	printf("------------------------------\n");
	for (i = 0; i < inst.c; i++){
		printf("#%d %d\n", i, inst.nlinhas[i]);
		for (j = 0; j < inst.nlinhas[i]; j++){
			printf("%d ", inst.coluna[i][j]);
		}
		printf("\n");
	}*/
	
	return 0;
}