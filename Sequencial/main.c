#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "aco.h"
#include "util.h"

char *nomeArquivo = "scp41.txt";

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
			inst.nlinhas[inst.linha[i][j]]++;   // contando numero de linhas que sao cobertas pela coluna
		}
	}

	inst.coluna = (int**)(malloc(inst.c * sizeof(int*)));
	int cont[inst.c];
	for (i = 0; i < inst.c; i++){
		inst.coluna[i] = (int*)(malloc(inst.nlinhas[i] * sizeof(int)));
		cont[i] = 0;
	}
	
	int k;
	for (i = 0; i < inst.l; i++){
		for (j = 0; j < inst.ncolunas[i]; j++){
			k = inst.linha[i][j];
			inst.coluna[k][cont[k]] = i;
			cont[k]++;
		}
	}

	fclose(arquivo);
	return inst;
}

void mostrarAjuda(){
	printf("\n\
	Uso: ./aco <nome_arquivo.txt> <opcoes>\n\
	-h, --help                  mostra tela de ajuda.\n\
	-v, --verbose               mostra informacoes detalhadas no decorrer do programa.\n\
	-a, --alfa=ALFA             [%.2f] seta a variavel alfa (importancia do feromonio) do algoritmo.\n\
	-b, --beta=BETA             [%.2f] seta a variavel beta (importancia da heuristica) do algoritmo.\n\
	-r, --rho=RHO               [%.2f] seta a variavel rho (taxa de evaporacao do feromonio) do algoritmo.\n\
	-f, --formigas=FORMIGAS     [%d] seta o numero de formigas do algoritmo.\n\
	-c, --ciclos=CICLOS         [%d] seta o numero de ciclos da condicao de parada do algoritmo.\n\n", alfa, beta, rho, n_formigas, n_ciclos) ;
	exit(-1);
}

void lerArgumentos(int argc, char *argv[]){
	int opt;

	if (argc < 2){
		mostrarAjuda();
	}

	const struct option opcoes[] = {
		{"help", no_argument, 0, 'h'},
		{"verbose", no_argument, 0, 'v'},
		{"alfa", required_argument, 0, 'a'},
		{"beta", required_argument, 0, 'b'},
		{"rho", required_argument, 0, 'r'},
		{"formigas", required_argument, 0, 'f'},
		{"ciclos", required_argument , 0, 'c'},
		{0, 0, 0, 0},
	};

	while((opt = getopt_long(argc, argv, "hva:b:r:f:c:", opcoes, NULL)) != -1) {
		switch (opt) {
			case 'h':
				mostrarAjuda();
				break;
			case 'v':
				verbose = true;
				break;
			case 'a':
				alfa = atof(optarg);
				break;
			case 'b':
				beta = atof(optarg);
				break;
			case 'r':
				rho = atof(optarg);
				break;
			case 'f':
				n_formigas = atoi(optarg);
				break;
			case 'c':
				n_ciclos = atoi(optarg);
				break;
			default:
				mostrarAjuda();
				return;
		}
	}

	if (optind < argc){
		nomeArquivo = argv[optind++];
	}
}

void inicializarInstancia(){
    char caminho[32] = "../Instancias/";
    strcat(caminho, nomeArquivo);

    instancia = lerArquivo(caminho);

    VERBOSE("\nDados da instancia:\n");
    VERBOSE("Arquivo: %s\nLinhas: %d\nColunas: %d\n", nomeArquivo, instancia.l, instancia.c);
    VERBOSE("\nParametros:\n");
    VERBOSE("Alfa = %.2f\n", alfa);
    VERBOSE("Beta = %.2f\n", beta);
    VERBOSE("Rho = %.2f\n", rho);
    VERBOSE("Numero de formigas = %d\n", n_formigas);
    VERBOSE("Numero de ciclos = %d\n\n", n_ciclos);
}

void freeInstancia(){
	free(instancia.custo);
	free(instancia.nlinhas);
	free(instancia.ncolunas);

	int i;
	for(i = 0; i < instancia.l; i++)
		free(instancia.linha[i]);

	for(i = 0; i < instancia.c; i++)
		free(instancia.coluna[i]);

	free(instancia.linha);
	free(instancia.coluna);
}

int main(int argc, char *argv[]){
	srand((unsigned)time(NULL));
	verbose = false;

	inicializar_parametros();
	lerArgumentos(argc, argv);
	inicializarInstancia();
	
	ant_colony();

	freeInstancia();
	
	return 0;
}