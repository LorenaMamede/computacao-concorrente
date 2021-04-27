/* Disciplina: Computacao Concorrente */
/* Aluna.: Lorena Mamede e Rodrigo Pita */
/* Trabalho: 1*/
/* Codigo: O usuario entra com um número, e o programa retorna seus divisores*/


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include"../Utilitarios/timer.h"

int numero; //entrada do usuario
int *divisores; //vetor que guardara os divisores encontrados
int indiceVetor=0; //variavel que registra a proxima posicao disponivel no vetor
int tamanhoVetor;
int numero_threads;
pthread_mutex_t mutex;

void * conta_divisores(void * arg){
	long int id = (long int) arg;
	int tamanhoBloco = tamanhoVetor/numero_threads; //define o tamanho do intervalo que a thread trabalhara
	int posicaoInicio = id * tamanhoBloco; //define por qual numero a thread comecara a sua busca
	int posicaoFim;//define a ultima posicao do numero com o qual a thread trabalhara

	if(id == numero_threads-1) posicaoFim = tamanhoVetor;
	else posicaoFim = posicaoInicio + tamanhoBloco;//percorre o vetor ate o fim, caso seja a ultima
	
	for(int i = posicaoInicio; i<posicaoFim; i++){
		if(i != 0 && (numero%i) == 0){ 
			pthread_mutex_lock(&mutex);//acionando o lock para mexer nas variaveis globais
			divisores[indiceVetor] = i; //guarda o divisor encontrado no vetor
			indiceVetor++; //sinaliza a proxima posicao disponivel
			pthread_mutex_unlock(&mutex);
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	double inicio, fim;
	pthread_t *tid;

        //verifica e recebe as entradas
	if(argc<3){
		printf("Digite: %s <numero que deseja saber o total de divisores> <numero de threads> \n", argv[0]);
		return 1;
	}
	numero = atoi(argv[1]);
	numero_threads = atoi(argv[2]);

	tamanhoVetor = (int) numero/2+1; //define a quantidade maxima de divisores
	divisores = (int *) malloc(sizeof(int)* tamanhoVetor);
	if(divisores==NULL) {
		printf("--ERRO: malloc");
		return 3;
	}

	tid = (pthread_t *) malloc(sizeof(pthread_t)*numero_threads);
	if(tid == NULL){
		printf("--ERRO: malloc");
		return 3;
	}

	GET_TIME(inicio);
	pthread_mutex_init(&mutex, NULL);

	for(int t=0; t<numero_threads; t++){
	        //printf("entrou na criacao da thread %d \n", t);
		if(pthread_create(tid+t, NULL, conta_divisores, (void *) t)){
			printf("--ERRO: pthread_creat()\n"); exit(-1);
		}
	}

	for(int j=0; j<numero_threads; j++){
		if(pthread_join(*(tid+j), NULL)){
			printf("--ERRO: pthread_join()\n");
			exit(-1);
		}
	}

	divisores[indiceVetor] = numero; //adiciona o proprio numero como divisor
	for(int k=0; k<indiceVetor+1; k++){
		printf("%d\n", *(divisores+k));
	}
	GET_TIME(fim);

	printf("Tempo de execucao concorrente com %d thread foi de %lf\n", numero_threads, fim-inicio);
	pthread_mutex_destroy(&mutex);
	free(divisores);
}
