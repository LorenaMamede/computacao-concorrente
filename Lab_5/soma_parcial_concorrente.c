/*Disciplina: Computacao Concorrente
* Aluna: Lorena Mamede
* Laboratorio: 5
* Codigo: Recebe vetor de entradas e aplica soma parcial em cada valor retornando o vetor obtido como saida
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int *vetorEntrada;
int *vetorTeste;
int elementosVetor = 0;
int tamanhoVetor = 2;
int leram=0;
int escreveram=0;
int threadsProximaFase=0;
int sinal = 1;

pthread_mutex_t mutex;
pthread_cond_t cond_prox;

/* Cria espaco em memoria para qualquer
* vetor de inteiros de acordo com o tamanho
* especificado
*/
int* alocarVetor(int tamanhoDesejadoVetor){
	int *vetor;
	vetor = (int *) malloc(sizeof(int)*tamanhoDesejadoVetor);
	if(vetor == NULL) {printf("--ERRO malloc\n");exit;}
	return vetor;
}


/* Cria um novo vetor para ser a versao
* redimensionada do vetor passado como
* argumento. O novo vetor possui o tamanho
* especificado na passagem e copia todos
* os elementos que haviam no vetor antigo.
*/
int* realocarVetor(int *vetor, int tamanhoDesejadoVetor){
	int *vetorNovo;
	vetorNovo = alocarVetor(tamanhoDesejadoVetor);
	for(int i = 0; i<tamanhoDesejadoVetor;i++){
		*(vetorNovo+i) = *(vetor+i);
	}

	return vetorNovo;
}

/* Recebe um vetor e uma entrada para transformar
* essa entrada em um inteiro e aloca-la no vetor
* fornecido. Caso o vetor tenha atingido seu limite,
* o redimensiona para futuras entradas,
* dobrando sua capacidade. Esse procedimento eh
* replicado em um vetor teste para conferencia
* ao final do programa
*/
void adicionarEntradaVetor(int *vetorEntrada, char entrada){
	int numeroEntrada;
	numeroEntrada = entrada - '0';
	*(vetorEntrada+elementosVetor) = numeroEntrada*sinal;
	*(vetorTeste+elementosVetor) = numeroEntrada*sinal;
	if(elementosVetor == tamanhoVetor-1){
		tamanhoVetor *= 2;
		vetorEntrada = realocarVetor(vetorEntrada, tamanhoVetor);
		vetorTeste = realocarVetor(vetorTeste, tamanhoVetor);
	}
	if(sinal<0) sinal=1;
	elementosVetor++;
}

/* Chamada para quando a thread nao pode mais
* operar no vetor. Ela deve se retirar das proximas
* rodadas decrementando a threadsProximaFase e
* notificar as demais sobre sua saida para que nao
* fiquem esperando por ela.
*/
void notificarSaida(long int id){
	pthread_mutex_lock(&mutex);
	threadsProximaFase--;
	pthread_cond_broadcast(&cond_prox);
//	printf("T%ld   saiu da fase e sinalizou para as outras!\n", id);
//	printf("T%ld   terminada, proxima fase apenas com %d threads\n",id,threadsProximaFase);
	pthread_mutex_unlock(&mutex);
}


/* Todas as threads que chegam nesta fase devem
* esperar por todas as outras que tambem estao
* cadastradas para a proxima fase antes de
* prosseguir para a fase de escrita. Nesta etapa de
* espera, alguma thread pode se retirar da proxima
* fase e quando isso acontece essa thread liberara
* as threads em espera para que elas revisem a
* quantidade de threads pelas quais devem esperar.
* Assim, eh evitado que as threads estejam esperando
* por threads que nao vao mais prosseguir. A ultima
* thread a chegar, deve liberar todas as demais para
* a etapa de escrita
*/
void sincronizarLeitura(long int id){
	if(leram==threadsProximaFase){
//		printf("SINCRONIZADAS: A ultima thread leu! Foi a %ld\n", id);
		pthread_cond_broadcast(&cond_prox);
	}

	while(leram<threadsProximaFase){
//		printf("T%ld   aguardando para proxima fase, apenas %d/%d threads leram\n", id, leram, threadsProximaFase);
		pthread_cond_wait(&cond_prox, &mutex);
	}

}

/* A mesma logica da sincronizacao de leitura eh
* aplicada aqui, porem nesta fase de espera, nao
* ha necessidade de checagem por desistencia, ja
* que apos a sincronizacao de leitura, todas as
* threads que chegaram ate aqui nao precisam
* realizar checagem de viabilidade para mexer no
* vetor. As threads sao liberadas apenas quando
* a ultima chega.
*/
void sincronizarEscrita(long int id){
	if(escreveram==threadsProximaFase){
//		printf("SINCRONIZADAS:  A ultima thread escreveu! Foi a %ld\n", id);
		pthread_cond_broadcast(&cond_prox);
		leram=0;//a variavel de leitura eh zerada apenas aqui pois aqui garantimos que
                        //todas as threads foram liberadas do hall de espera da sincronizacao de leitura
	}

	if(escreveram<threadsProximaFase){
//		printf("T%ld   aguardando para proxima fase, apenas %d/%d threads escreveram\n", id, escreveram, threadsProximaFase);
		pthread_cond_wait(&cond_prox, &mutex);
	}

}

void * calcularSomaParcial(void* args){
	long int id = (long int) args;
	int varAux;
	for(int i=1; i<tamanhoVetor;i*=2){
		escreveram=0;

		printf("T%ld   iniciada ----------- loop %d\n",id,i);

		if(id-i< 0) { notificarSaida(id); break;}//area de desistencia para threads que nao conseguem
							 //mais acessar indices possiveis no vetor

		pthread_mutex_lock(&mutex);

		varAux = *(vetorEntrada+id-i);
		leram++;
//		printf("T%ld   leu %d\n", id,varAux);
		sincronizarLeitura(id);

		*(vetorEntrada+id) += varAux;
//		printf("T%ld   escreveu %d\n", id, *(vetorEntrada+id));
		escreveram++;
		sincronizarEscrita(id);

		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

/*Saida dos resultados da soma parcial*/
void mostrarResultado(){
	puts("\nSoma parcial de cada posicao:");

	printf("[");
	for(int j = 0; j < elementosVetor; j++){
		printf("%d,", *(vetorEntrada+j));
	}
	printf("]\n");

}

/* Checagem do resultado obtido com as threads.
* Eh feita com a comparacao da soma parcial
* sequencial aplicada ao vetor teste (vetor clone
* do vetor de entrada).
*/
void testarResultado(){
	puts("\nRodando testes para verificar resultados...");
	puts("Comparando resultado com vetor de testes...");
	for(int j =1;j<elementosVetor;j++){
		*(vetorTeste+j) += *(vetorTeste+j-1);
		if(*(vetorTeste+j) == *(vetorEntrada+j)){
			printf("SUCESSO na posicao %d! %d = %d \n", j, *(vetorTeste+j), *(vetorEntrada+j));
		}else{
			printf("FALHA na posicao %d! %d != %d \n", j,*(vetorTeste+j), *(vetorEntrada+j));

		}
	}

}

int main(){
	char entrada;
	pthread_t *tid;

	vetorEntrada = alocarVetor(tamanhoVetor);
	vetorTeste = alocarVetor(tamanhoVetor);

	puts("Forneca um vetor de entrada inserindo os numero separados por espaço\nExemplo: 1 2 3 4\n");
	while(scanf("%c", &entrada) && (entrada != '\n')){//aceita entradas ate receber enter
		if(entrada == ' ') continue;
		if(entrada == '-') {sinal *= -1; continue;}//artificio para entrada de numero negativos nao ser
                                                           //considerada entrada de dois numeros
		adicionarEntradaVetor(vetorEntrada, entrada);//inclui o numero no vetor
	}

	threadsProximaFase = elementosVetor;//define quantidade de threads para a primeira fase
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_prox, NULL);

	tid = (pthread_t *) malloc(sizeof(pthread_t)*elementosVetor);
	if(tid==NULL){printf("Erro malloc\n"); exit(-1);}

	for(int i = 0; i<elementosVetor; i++){
		if(pthread_create((tid+i), NULL, calcularSomaParcial, (void *) i)){ 
			printf("Erro pthread_create\n");
			exit(-1);}
	}

	for(int i = 0; i<elementosVetor;i++){
		if(pthread_join(*(tid+i), NULL)) exit(-1);
	}

	mostrarResultado();
	testarResultado();

	pthread_mutex_destroy(&mutex);
	free(vetorEntrada);
	free(tid);
}
