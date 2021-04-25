/* Disciplina: Computacao Concorrente */
/* Aluna.: Lorena Mamede e Rodrigo Pita */
/* Trabalho: 1*/
/* Codigo: O usuario entra com um número, e o programa retorna seus divisores*/


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

int numero; //entrada do usuario
int *divisores;
int indiceVetor=0;
int tamanhoVetor;
int numero_threads;
pthread_mutex_t mutex;
int global = 1;

void * conta_divisores(void * arg){
	long int id = (long int) arg;
	int tamanhoBloco = tamanhoVetor/numero_threads;
	int posicaoInicio = id * tamanhoBloco;
	int posicaoFim;

	if(id == numero_threads-1) posicaoFim = tamanhoVetor;
	else posicaoFim = posicaoInicio + tamanhoBloco;
	
	for(int i = posicaoInicio; i<posicaoFim; i++){
		pthread_mutex_lock(&mutex);
		if(i != 0 && (numero%i) == 0){
			divisores[indiceVetor] = i;
			indiceVetor++;
		}
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t tid[numero_threads];
	if(argc<3){
		printf("Digite: %s <numero que deseja saber o total de divisores> <numero de threads> \n", argv[0]);
		return 1;
	}
	numero = atoi(argv[1]);
	numero_threads = atoi(argv[2]);

	tamanhoVetor = (int) numero/2+1;
	divisores = (int *) malloc(sizeof(int)* tamanhoVetor);

	pthread_mutex_init(&mutex, NULL);
	for(int t=0; t<numero_threads; t++){
	    //printf("entrou na criacao da thread %d \n", t);
		if(pthread_create(&tid[t], NULL, conta_divisores, (void *) t)){
			printf("--ERRO: pthread_creat()\n"); exit(-1);
		}
	}

	for(int t=0; t<numero_threads; t++){
		if(pthread_join(tid[t], NULL)){
			printf("--ERRO: pthread_join()\n");
			exit(-1);
		}
	}

	divisores[indiceVetor] = numero;
	for(int t=0; t<indiceVetor+1; t++){
		printf("%d\n", *(divisores+t));
	}

	pthread_mutex_destroy(&mutex);
}
