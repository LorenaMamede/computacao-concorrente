#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
#define NELEMENTS 10000

int vetor[NELEMENTS];

//funcao executada pelas threads
void *AddOne (void *arg) {
  int indiceComeco = *(int *) arg;

  for(int i = indiceComeco; i<NELEMENTS;i+=NTHREADS){
      vetor[i]++;
  }

  pthread_exit(NULL);
}

int main() {
  pthread_t tid_sistema[NTHREADS];
  int vetorComparacao[NELEMENTS];
  int indiceComeco;
  int elementosTestados;

  //incializando o vetor com 0
  for(int i = 0; i < NELEMENTS; i++){
      vetor[i] = 0;
      vetorComparacao[i] = vetor[i] + 1;
  }

  //criando threads informando posicao de inicio da soma
  for(int thread=0; thread<NTHREADS; thread++) {
    printf("--Cria a thread %d que comecara a soma a partir de %d\n", thread, thread);
    indiceComeco = thread;
    if (pthread_create(&tid_sistema[thread], NULL, AddOne, (void *) &indiceComeco)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //esperando todas as threads terminarem
  for (int thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
    }
  }

  //testando saida
  for(elementosTestados = 0; elementosTestados <NELEMENTS; elementosTestados++){
      if(vetor[elementosTestados] != vetorComparacao[elementosTestados]){
          printf("ERROR: A soma não ocorreu como esperado\n");
          break;
      }
  }

  if(elementosTestados==NELEMENTS) printf("PASSED: A soma ocorreu com sucesso!");

  printf("\n--Thread principal terminou\n");
  pthread_exit(NULL);
}