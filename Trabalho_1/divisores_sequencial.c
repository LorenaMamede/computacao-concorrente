/* Disciplina: Computacao Concorrente */
/* Aluna.: Lorena Mamede e Rodrigo Pita */
/* Trabalho: 1*/
/* Codigo: O usuario entra com um número, e o programa retorna seus divisores*/


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include"../Utilitarios/timer.h"

int numero; //entrada do usuario
int *divisores; //vetor que guardara os divisores encontrados
int indiceVetor; //variavel que registra a proxima posicao disponivel no vetor
int tamanhoVetor;

void conta_divisores(){
	for(int i = 1; i<tamanhoVetor; i++){
		if((numero%i) == 0){ 
			divisores[indiceVetor] = i; //guarda o divisor encontrado no vetor
			indiceVetor++; //sinaliza a proxima posicao disponivel
		}
	}
}

int main(int argc, char *argv[]){
	double inicio, fim;

        //verifica e recebe as entradas
	if(argc<2){
		printf("Digite: %s <numero que deseja saber o total de divisores> \n", argv[0]);
		return 1;
	}
	numero = atoi(argv[1]);

	tamanhoVetor = (int) numero/2+1; //define a quantidade maxima de divisores
	divisores = (int *) malloc(sizeof(int)* tamanhoVetor);
	if(divisores==NULL) {
		printf("--ERRO: malloc");
		return 3;
	}


	GET_TIME(inicio);

	conta_divisores();
	divisores[indiceVetor] = numero; //adiciona o proprio numero como divisor
	for(int k=0; k<indiceVetor+1; k++){
		printf("%d\n", *(divisores+k));
	}
	free(divisores);
	GET_TIME(fim);
	printf("Tempo de execucao sequencial foi de %lf\n", fim-inicio);
	return 0;
}
