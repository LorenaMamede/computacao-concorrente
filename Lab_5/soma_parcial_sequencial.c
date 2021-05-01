#include<stdio.h>
#include<stdlib.h>

int* alocarVetor(int tamanhoDesejadoVetor){
	int *vetor;
	vetor = (int *) malloc(sizeof(int)*tamanhoDesejadoVetor);
	if(vetor == NULL) {printf("--ERRO malloc\n");exit;}
	return vetor;
}


int* realocarVetor(int *vetor, int tamanhoDesejadoVetor){
	int *vetorNovo;
	vetorNovo = alocarVetor(tamanhoDesejadoVetor);
	for(int i = 0; i<tamanhoDesejadoVetor;i++){
		*(vetorNovo+i) = *(vetor+i);
	}

	return vetorNovo;
}


int main(){
	int *vetorEntrada;
	int posicaoVetor = 0;
	int tamanhoVetor = 2;
	char entrada;

	vetorEntrada = alocarVetor(tamanhoVetor);
	while(scanf("%c", &entrada) && (entrada != '\n')){
		if(entrada == ' ') continue;
		*(vetorEntrada+posicaoVetor) = entrada - '0';
		if(posicaoVetor == tamanhoVetor-1){
			tamanhoVetor *= 2;
			vetorEntrada = realocarVetor(vetorEntrada, tamanhoVetor);
		}
		posicaoVetor++;
	}

	for(int j = 0; j < posicaoVetor; j++){
		printf("%d, ", *(vetorEntrada+j));
	}

	return 0;
}
