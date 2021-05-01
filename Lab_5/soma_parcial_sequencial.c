#include<stdio.h>
#include<stdlib.h>

int posicaoVetor = 0;
int tamanhoVetor = 2;
int sinal = 1;

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

void adicionarEntradaVetor(int *vetorEntrada, char entrada){
	int numeroEntrada;
	numeroEntrada = entrada - '0';
	*(vetorEntrada+posicaoVetor) = numeroEntrada*sinal;
	if(posicaoVetor == tamanhoVetor-1){
		tamanhoVetor *= 2;
		vetorEntrada = realocarVetor(vetorEntrada, tamanhoVetor);
	}
	if(sinal<0) sinal=1;
	posicaoVetor++;
}


int main(){
	int *vetorEntrada;
	char entrada;

	vetorEntrada = alocarVetor(tamanhoVetor);

	puts("Forneca um vetor de entrada inserindo os numero separados por espaço\nExemplo: 1 2 3 4\n");
	while(scanf("%c", &entrada) && (entrada != '\n')){
		if(entrada == ' ') continue;
		if(entrada == '-') {sinal *= -1; continue;}
		adicionarEntradaVetor(vetorEntrada, entrada);
	}


	for(int i=1; i<posicaoVetor;i++){
		*(vetorEntrada+i) += *(vetorEntrada+(i-1));
	}
	puts("Soma parcial de cada posicao:");
	for(int j = 0; j < posicaoVetor; j++){
		printf("%d, ", *(vetorEntrada+j));
	}
	puts("");

	return 0;
}
