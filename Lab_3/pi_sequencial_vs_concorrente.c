#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include"../Utilitarios/timer.h"

int numero_threads;
long long int numero_elementos;

void * calcula_pi(void * arg){
	long int id = (long int) arg;
	int tamanho_bloco = numero_elementos/numero_threads;
	int elemento_inicial = id * tamanho_bloco;
	int elemento_final;
	if(id == numero_threads-1) elemento_final = numero_elementos;
	else elemento_final = elemento_inicial + tamanho_bloco;

	double *soma_pi;
	soma_pi = (double *) malloc(sizeof(double));
	if(soma_pi == NULL){printf("ERRO--malloc\n"); exit(1);}
	*soma_pi=0;

	int denominador = 1 + 2 * elemento_inicial;
	int parcelas = elemento_inicial;
	while(parcelas < elemento_final){
		double parcela = 4.0/denominador;
		if(parcelas%2 == 1){
			parcela *= -1;
		}
		*soma_pi += parcela;
		denominador += 2;
		parcelas++;
	}

	pthread_exit((void *) soma_pi);

}

int main(int argc, char *argv[]){
    double inicio, fim;
	pthread_t *tid; //identificacao das threads
	double *retorno; //retorno da soma parcial de pi  de cada thread
	double soma_pi_conc; //soma global
    double soma_pi_seq; //soma sequencial

	//validar e capturar entradas
	if(argc<3){
		printf("Digite: %s <numero de elementos> <numero de threads>\n", argv[0]);
		return 1;
	}
	numero_elementos = atoll(argv[1]);
	numero_threads = atoi(argv[2]);
	if(numero_threads > numero_elementos) numero_threads = numero_elementos; 


    //calcular pi sequencialmente
    GET_TIME(inicio);
    int parcelas=0;
	int denominador = 1;
	while(parcelas < numero_elementos){
		double parcela = 4.0/denominador;
		if(parcelas%2 == 1){
			parcela *= -1;
		}
		soma_pi_seq += parcela;
		denominador += 2;
		parcelas++;
	}

	//exibir resultados sequenciais
    GET_TIME(fim);
    double tempo_sequencial = fim -inicio;
    printf("Tempo de execucao sequencial: %lf\n", tempo_sequencial);
	printf("Pi calculado sequencialmente para %lld elementos eh %.15lf\n", numero_elementos, soma_pi_seq);
	printf("Pi fornecido pela M_PI %.15lf\n", M_PI);
	printf("O erro foi de %.15lf\n", M_PI - soma_pi_seq);

	//alocar memorica
	tid = (pthread_t *) malloc(sizeof(pthread_t) * numero_threads);
	if(tid == NULL) { printf("ERRO--malloc\n"); return 2;}

	//criar threads
    GET_TIME(inicio);
	for(int i=0; i<numero_threads; i++){
		if(pthread_create(tid+i,NULL, calcula_pi, (void *) i)){
			printf("ERRO--pthread_create\n");
			return 3;
		}
	}

	//esperar as threads terminarem e somar os resultados
	for(int i=0; i<numero_threads; i++){
		if(pthread_join(*(tid+i),(void **) &retorno)){
			printf("ERRO--pthread_join\n");
			return 3;
		}
		soma_pi_conc += *retorno;
		free(retorno);
	}
    GET_TIME(fim);
    double tempo_concorrente = fim -inicio;
    printf("\n\nTempo de execucao concorrente: %lf\n", tempo_concorrente);

	//exibir resultados
	printf("Pi calculado concorrentemente para %lld elementos eh %.15lf\n", numero_elementos, soma_pi_conc);
	printf("Pi fornecido pela M_PI %.15lf\n", M_PI);
	printf("O erro foi de %.15lf\n", M_PI - soma_pi_conc);

	free(tid);

    printf("\n\nGanho de desempenho foi de: %lf\n", tempo_sequencial/tempo_concorrente);
}
