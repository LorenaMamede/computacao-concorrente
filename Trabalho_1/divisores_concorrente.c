#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NTHREADS 2
int numero; //entrada do usuario

void * conta_divisores(void * arg){
	long int id = (long int) arg;
	printf("Sou a thread %ld\n", id);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS];
	if(argc<2){
		printf("Digite: %s <numero que deseja saber o total de divisores> \n", argv[0]);
		return 1;
	}
	numero = atoi(argv[1]);

	for(int t=0; t<NTHREADS; t++){
	        printf("entrou na criacao da thread %d \n", t);
		if(pthread_create(&tid[t], NULL, conta_divisores, (void *) t)){
			printf("--ERRO: pthread_creat()\n"); exit(-1);
		}
	}

	for(int t; t<NTHREADS; t++){
		if(pthread_join(tid[t], NULL)){
			printf("--ERRO: pthread_join()\n");
			exit(-1);
		}
	}

	pthread_exit(NULL);
}
