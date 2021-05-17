#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "../../projetos/computacao-concorrente/Utilitarios/timer.h"

long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
int *vetor; //vetor de entrada com dimensao dim


//fluxo principal
int main(int argc, char *argv[]){
	long int somaSeq = 0;//soma sequencial
	long int somaConc = 0;//soma concorrente
	double inicio, fim, delta;//tomada de tempo
	
	//recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
	if(argc < 3){
		fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);

	//aloca o vetor de entrada
	vetor = (int*) malloc(sizeof(int)*dim);
	if(vetor == NULL) {
		fprintf(stderr, "Erro--malloc\n");
		return 2;
	}

	//preenche o vetor de entrada
	for(long int i=0;i<dim; i++){
		vetor[i] = i%1000;
	}

	//soma sequencial dos elementos
	GET_TIME(inicio);
	for(long int i=0; i<dim;i++){
		somaSeq += vetor[i];
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo sequencial: %lf\n", delta);
	//soma concorrente dos elementos
	//criar as threads

	//aguardar o termino das threads

	//computar o valor final

	//exibir os resultados
	printf("Soma sequencial: %ld\n", somaSeq);
	printf("Soma concorrente: %ld\n", somaConc);


	//libera as areas de memoria alocadas
	free(vetor);
	return 0;
}
