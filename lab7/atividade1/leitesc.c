/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Descricao: implementa  o problema dos leitores/escritores usando variaveis de condicao da biblioteca Pthread
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define L 2 //numero de threads leitoras
#define E 2 //numero de threads escritoras

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

//entrada leitura
void InicLeit (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] quer ler\n", id);
   while(escr > 0) {
     printf("L[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_leit, &mutex);
     printf("L[%d] desbloqueou\n", id);
   }
   leit++;
   printf("L[%d] pode ler\n", id);
   pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] terminou de ler\n", id);
   leit--;
   if(leit==0) pthread_cond_signal(&cond_escr);
   pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] quer escrever\n", id);
   while((leit>0) || (escr>0)) {
     printf("E[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_escr, &mutex);
     printf("E[%d] desbloqueou\n", id);
   }
   escr++;
   printf("E[%d] pode escrever\n", id);
   pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] terminou de escrever\n", id);
   escr--;
   pthread_cond_signal(&cond_escr);
   pthread_cond_broadcast(&cond_leit);
   pthread_mutex_unlock(&mutex);
}

//thread leitora
void * leitor (void * arg) {
  int *id = (int *) arg;
  int boba=0;
  while(1) {
    InicLeit(*id);
    for(int i=-100000; i<10000; i++) boba++;
    FimLeit(*id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

//thread escritora
void * escritor (void * arg) {
  int *id = (int *) arg;
  int boba=0;
  while(1) {
    InicEscr(*id);
    for(int i=-100000; i<10000; i++) boba++;
    FimEscr(*id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  //inicializa as variaveis de sincronizacao
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_leit, NULL);
  pthread_cond_init(&cond_escr, NULL);

  //cria as threads leitoras
  for(int i=0; i<L; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
  } 
  
  //cria as threads escritoras
  for(int i=0; i<E; i++) {
    id[i+L] = i+1;
    if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
  } 

  pthread_exit(NULL);
  return 0;
}
