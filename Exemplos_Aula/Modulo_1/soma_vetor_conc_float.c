#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "../../projetos/computacao-concorrente/Utilitarios/timer.h"

long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
float *vetor; //vetor de entrada com dimensao dim

//fluxo das threads
void * tarefa (void * arg){
	long int id = (long int) arg; //identificador da thread
	float *somaLocal; //variavel local da soma de elementos
	long int tamBloco = dim/nthreads; //tamanho do bloco de cada thread
	long int ini = id  * tamBloco; //elemento incial do bloco da thread
	long int fim; //elemento final (nao processado) do bloco da thread
	somaLocal = (float *) malloc(sizeof(float));
	if(somaLocal == NULL){
		fprintf(stderr, "Erro-malloc\n"); exit(1);
	}
	*somaLocal = 0;

	if(id == nthreads-1) fim = dim;
	else fim = ini + tamBloco; //trata o resto se houver

	//soma os elementos do bloco da thread
	for(long int i = ini; i<fim;i++){
		*somaLocal += vetor[i];
	}
	//retorna o resultado da soma local
	pthread_exit((void *) somaLocal);
}

//fluxo principal
int main(int argc, char *argv[]){
	float somaSeq = 0;//soma sequencial
	float somaConc = 0;//soma concorrente
	double inicio, fim, delta;//tomada de tempo
	pthread_t *tid; //identificadores das threads no sistema
	float *retorno; //valor de retorno das threads
	//recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
	if(argc < 3){
		fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);

	//aloca o vetor de entrada
	vetor = (float*) malloc(sizeof(float)*dim);
	if(vetor == NULL) {
		fprintf(stderr, "Erro--malloc\n");
		return 2;
	}

	//preenche o vetor de entrada
	for(long int i=0;i<dim; i++){
		vetor[i] = 1000.1/(i+1); //somar 1 ao i para nao acontecer divisao por 0
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
	GET_TIME(inicio);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL){
		fprintf(stderr, "Erro--malloc\n");
		return 2;
	}

	//criar as threads
	for(int i=0; i<nthreads; i++){
		if(pthread_create(tid+i, NULL, tarefa, (void *) i)){
			fprintf(stderr, "ERRO--pthread_create\n");
			return 3;
		}
	}

	//aguardar o termino das threads
	for(int i=0; i<nthreads; i++){
		if(pthread_join(*(tid+i),(void **) &retorno)){ //passa  o endereco que sera guardado o endereco
			fprintf(stderr, "ERRO--pthread_join\n");
			return 3;
		}
		//soma global
		somaConc += *retorno;
		free(retorno);
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo concorrente: %lf\n",delta);
 
	//exibir os resultados
	printf("Soma sequencial: %f\n", somaSeq);
	printf("Soma concorrente: %f\n", somaConc);


	//libera as areas de memoria alocadas
	free(vetor);
	free(tid);
	return 0;
}
