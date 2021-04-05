/* Author: Lorena Mammede */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

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

	/*mat = (float *) malloc(sizeof(float) * dimensao_matriz * dimensao_matriz);
	if(mat == NULL) {printf("ERRO: malloc\n"); return 2;}*/
	return 1;
}

void conferir_operacoes(){
	puts("\nA matriz foi resultante foi: \n");
	for(int linha=0; linha<dimensao_matriz; linha++){
		for(int coluna=0; coluna<dimensao_matriz; coluna++){
			printf("%.1f  ", matriz_resultante[linha*dimensao_matriz + coluna]);
		}
		puts("");
	}

}

void *  multiplicar_matrizes(void *arg){
	int linha_thread = *(int *) arg;
	printf("Thread %d\n", linha_thread);
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

	//verificacao e captura dos parametros de entrada
	if(argc<3){
		printf("Erro ao ler argumentos, entre com %s <dimensao da matriz> <numero de threads>\n", argv[0]);
		return 1;
	}

	dimensao_matriz = atoi(argv[1]);
	numero_threads = atoi(argv[2]);

	if(numero_threads > dimensao_matriz) numero_threads = dimensao_matriz;

	//alocacao de memoria
	if(alocar_memoria_entradas()){
		//incializacao 
		inicializar_matriz();
		
		tid_local = (int *) malloc(sizeof(int) * numero_threads);
		if(tid_local == NULL) return 2;

		//criacao de threads e multiplicacao de matrizes
		for(int thread=0; thread<numero_threads; thread++){
			printf("Criando thread %d\n", thread);	
			*(tid_local + thread) = thread; //linha pela qual cada thread comecara
			if(pthread_create(tid+thread, NULL, multiplicar_matrizes, (void *) (tid_local+thread))){
				puts("ERRO ao executar pthread_create"); return 3;
			}
		}
	
		for(int thread=0; thread<numero_threads; thread++){
			pthread_join(*(tid+thread), NULL);
		}

		//printa na tela a matriz resultante
		conferir_operacoes();
	}else{
		return 2;
	}
	free(matriz);
	free(tid);
	free(tid_local);
	free(matriz_resultante);
	return 0;
}
