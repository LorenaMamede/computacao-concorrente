#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

int main(int argc, char *argv[]){
	long long int numero_elementos;
	double soma_pi;
	int parcelas;

	//validar e capturar entradas
	if(argc<2){
		printf("Digite: %s <numero de elementos>\n", argv[0]);
		return 1;
	}
	numero_elementos = atoll(argv[1]);

	//calcular pi sequencialmente
	int denominador = 1;
	while(parcelas < numero_elementos){
		double parcela = 4.0/denominador;
		if(parcelas%2 == 1){
			parcela *= -1;
		}
		soma_pi += parcela;
		denominador += 2;
		parcelas++;
	}

	//exibir resultados
	printf("Pi calculado para %lld elementos eh %.15lf\n", numero_elementos, soma_pi);
	printf("Pi fornecido pela M_PI %.15lf\n", M_PI);
	printf("O erro foi de %.15lf\n", M_PI - soma_pi);
}
