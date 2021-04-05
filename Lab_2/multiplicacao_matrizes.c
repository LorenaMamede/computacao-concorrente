/* Author: Lorena Mammede */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>
#include "timer.h"

int numero_threads;
float *matriz;
float *matriz_resultante;
pthread_t *tid;
int dimensao_matriz;


void inicializar_matriz(){
	for(int linha=0; linha<dimensao_matriz; linha++){
		for(int coluna=0; coluna<dimensao_matriz; coluna++){
			matriz[linha*dimensao_matriz + coluna] = 1;
			matriz_resultante[linha*dimensao_matriz + coluna] = 0;
		}
	}

}

int alocar_memoria_entradas(){
	matriz = (float *) malloc(sizeof(float) * dimensao_matriz * dimensao_matriz);
	if(matriz == NULL) {printf("ERRO: malloc\n"); return 0;}
	matriz_resultante = (float *) malloc(sizeof(float) * dimensao_matriz * dimensao_matriz);
	if(matriz_resultante == NULL) {printf("ERRO: malloc\n"); return 0;}
	tid = (pthread_t *) malloc(sizeof(pthread_t *) * numero_threads);
	if(tid == NULL) {printf("ERRO: malloc\n"); return 0;}

	return 1;
}

void conferir_operacoes(){
	bool sucesso;
//	puts("\nA matriz resultante foi: \n");
	for(int linha=0; linha<dimensao_matriz; linha++){
		for(int coluna=0; coluna<dimensao_matriz; coluna++){
			//printf("%.1f  ", matriz_resultante[linha*dimensao_matriz + coluna]);
			if(matriz_resultante[linha * dimensao_matriz + coluna] == dimensao_matriz)
				sucesso = true;
		}
	//	puts("");
	}
	puts("\nEfetuando testes...");
	if(sucesso) puts("SUCESSO: o resultado está correto!\n");
	else puts("ERRO: resultado inesperado!\n");
}

void *  multiplicar_matrizes(void *arg){
	int linha_thread = *(int *) arg;
	//printf("Thread %d\n", linha_thread);
	for(int linha=linha_thread; linha<dimensao_matriz; linha+=numero_threads){
		for(int coluna=0; coluna<dimensao_matriz; coluna++){
			for(int parcela=0; parcela<dimensao_matriz; parcela++){
				int indice = linha * dimensao_matriz + coluna;
				matriz_resultante[indice] += matriz[linha*dimensao_matriz + parcela]  * matriz[indice];
			}
		}
	}
}

int main(int argc, char* argv[]){
	int *tid_local;
	double inicio, fim, delta;

	//verificacao e captura dos parametros de entrada
	if(argc<3){
		printf("Erro ao ler argumentos, entre com %s <dimensao da matriz> <numero de threads>\n", argv[0]);
		return 1;
	}

	dimensao_matriz = atoi(argv[1]);
	numero_threads = atoi(argv[2]);
	
	if(numero_threads > dimensao_matriz) numero_threads = dimensao_matriz;
	
	puts("--------------------------------------------------");
	printf("A dimensao escolhida foi %d e %d thread(s)\n", dimensao_matriz, numero_threads);
	//alocacao de memoria
	if(alocar_memoria_entradas()){
		
		//incializacao 
		GET_TIME(inicio);
		inicializar_matriz();
		GET_TIME(fim);
		delta = fim - inicio;
		printf("Tempo inicializacao: %lf\n", delta);

		tid_local = (int *) malloc(sizeof(int) * numero_threads);
		if(tid_local == NULL) return 2;

		//criacao de threads e multiplicacao de matrizes
		GET_TIME(inicio);
		for(int thread=0; thread<numero_threads; thread++){
			//printf("Criando thread %d\n", thread);	
			*(tid_local + thread) = thread; //linha pela qual cada thread comecara
			if(pthread_create(tid+thread, NULL, multiplicar_matrizes, (void *) (tid_local+thread))){
				puts("ERRO ao executar pthread_create\n"); return 3;
			}
		}
	
		for(int thread=0; thread<numero_threads; thread++){
			pthread_join(*(tid+thread), NULL);
		}

		GET_TIME(fim);
		delta = fim -inicio;
		printf("Tempo criacao e multiplicacao: %lf\n", delta);

		//printa na tela a matriz resultante
		conferir_operacoes();
	}else{
		return 2;
	}

	GET_TIME(inicio);
	free(matriz);
	free(tid);
	free(tid_local);
	free(matriz_resultante);
	GET_TIME(fim);
	delta = fim - inicio;

	printf("Tempo finalizacao: %lf\n", delta);

	return 0;
}
