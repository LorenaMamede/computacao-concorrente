/* Disciplina: Computacao Concorrente */
/* Aluna.: Lorena Mamede */
/* Laboratório: 4 */

/***** Condicao logica da aplicacao:   ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 4 */
void *Quatro (void *t) {
  printf("T4: Seja bem-vindo!\n");
  
  pthread_mutex_lock(&x_mutex);
  x=4; //guarda o numero da thread
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread 2 */
void *Dois (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x==1 || x==0) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  if(x==3) x=5;
  else x=2;
  printf("T2: Fique a vontade.\n");
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread 3 */
void *Tres (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x==1 || x==0) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  if(x==2) x=5;
  else x=3;
  printf("T3: Sente-se por favor.\n");
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}
/* Thread 1 */
void *Um (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x !=5) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  x=1;
  printf("T1: Volte sempre!\n");
  pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[3], NULL, Quatro, NULL);
  pthread_create(&threads[2], NULL, Dois, NULL);
  pthread_create(&threads[1], NULL, Tres, NULL);
  pthread_create(&threads[0], NULL, Um, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
