/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

float *mat; //matriz de entrada
float *vet; //vetor de entrada
float *saida; //vetor de saida

typedef struct{
	int linha; //identifica a linha que a thread vai trabalhar
	int dim; //identifica a dimensao das estruturas de entrada
} tArgs;

void * tarefa(void *args){
	tArgs = (tArgs *) args;

	for(int j=0; j<args->dim; j++)
		saida[args->linha] += mat[(args->linha) * (args->dim) + j] * vet[j];
	}
}

int main(int argc, char* argv[]){
	int dim; //dimensao da matriz de entrada
	tArgs targs;
	pthread_t tid;

	//leitura e avaliacao dos parametros de entrada
	if(argc<2){
		printf("Digite: %s <dimensao da matriz>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);

	//alocacao de memoria para as estruturas de dados
	mat = (float *) malloc(sizeof(float) * dim * dim);
	if(mat == NULL) {printf("ERRO--malloc\n"); return 2;}
	vet = (float *)  malloc(sizeof(float) * dim);
	if(vet == NULL) {printf("ERRO--malloc\n"); return 2;}
	saida = (float *)  malloc(sizeof(float) * dim);
	if(saida == NULL) {printf("ERRO--malloc\n"); return 2;}
	tid = (pthread_t *) malloc(sizeof(pthread) * dim);	
	if(tid == NULL) {printf("ERRO--malloc\n"); return 2;}
	targs = (tArgs) malloc(sizeof(tArgs) * dim);
	if(targs == NULL) {printf("ERRO--malloc\n"); return 2;}

	//inicializacao das estruturas de dados de entrada e saida
	for(int i=0; i<dim; i++){
		for(int j=0; j<dim; j++)
			mat[i*dim+j] = 1; //mat[i][j]
		vet[i] = 1;
		saida[i] = 0;
	}

	//multiplicacao da matriz pelo vetor
	for(int i=0; i<dim; i++){
		pthread_create(NULL, tarefa,);
	}

	//exibicao dos resultados
	/*for(int i=0; i<dim; i++){
		for(int j=0; j<dim; j++)
			printf("%.1f ", mat[i*dim+j]);
		puts("");
	}
	puts("Vetor de entrada: ");

	for(int j=0; j<dim; j++)
		printf("%.1f ", vet[j]);
	puts("");
	puts("Vetor de entrada: ");*/

	for(int j=0; j<dim; j++)
		printf("%.1f ", saida[j]);
	puts("");

	//liberacao da memoria

	free(mat);
	free(vet);
	free(saida);

	return 0;
}
