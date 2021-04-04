/* Author: Lorena Mammede */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int numero_threads;
float *matriz;
float *matriz_resultante;

void inicializar_matriz(int dimensao_matriz){
	for(int linha=0; linha<dimensao_matriz; linha++){
		for(int coluna=0; coluna<dimensao_matriz; coluna++){
			matriz[linha*dimensao_matriz + coluna] = 1;
			matriz_resultante[linha*dimensao_matriz + coluna] = 0;
		}
	}

}

int alocar_memoria_entradas(int dimensao_matriz){
	matriz = (float *) malloc(sizeof(float) * dimensao_matriz * dimensao_matriz);
	if(matriz == NULL) {printf("ERRO: malloc\n"); return 0;}
	matriz_resultante = (float *) malloc(sizeof(float) * dimensao_matriz * dimensao_matriz);
	if(matriz_resultante == NULL) {printf("ERRO: malloc\n"); return 0;}

	/*mat = (float *) malloc(sizeof(float) * dimensao_matriz * dimensao_matriz);
	if(mat == NULL) {printf("ERRO: malloc\n"); return 2;}*/
	return 1;
}

void conferir_operacoes(int dimensao_matriz){
	puts("A matriz foi inicializada como: \n");
	for(int linha=0; linha<dimensao_matriz; linha++){
		for(int coluna=0; coluna<dimensao_matriz; coluna++){
			printf("%.1f  ", matriz_resultante[linha*dimensao_matriz + coluna]);
		}
		puts("");
	}

}

void multiplicar_matrizes(int dimensao_matriz){
	for(int linha=0; linha<dimensao_matriz; linha++){
		for(int coluna=0; coluna<dimensao_matriz; coluna++){
			for(int parcela=0; parcela<dimensao_matriz; parcela++){
				int indice = linha * dimensao_matriz + coluna;
				matriz_resultante[indice] += matriz[linha*dimensao_matriz + parcela]  * matriz[indice];
			}
		}
	}
}

int main(int argc, char* argv[]){
	int dimensao_matriz;

	//verificacao e captura dos parametros de entrada
	if(argc<3){
		printf("Erro ao ler argumentos, entre com %s <dimensao da matriz> <numero de threads>\n", argv[0]);
		return 1;
	}

	dimensao_matriz = atoi(argv[1]);
	numero_threads = atoi(argv[2]);

	if(numero_threads > dimensao_matriz) numero_threads = dimensao_matriz;

	//alocacao de memoria
	if(alocar_memoria_entradas(dimensao_matriz)){
		//incializacao 
		inicializar_matriz(dimensao_matriz);
		
		//multiplicacao de matrizes
		multiplicar_matrizes(dimensao_matriz);

		//confere
		conferir_operacoes(dimensao_matriz);

		return 0;
	}else{
		return 2;
	}
}
