/* 
Programa: Programação Concorrente
Descricao: implementa  uma variação a partir de buffer do problema dos produtores/consumidores usando variaveis de condicao da biblioteca Pthread
Autor: Samuel Sampaio
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define N 5
#define P 2
#define C 4

//variveis do problema 
int Buffer[N];
int count = 0;

//variáveis para sincronização
pthread_mutex_t mutex;
pthread_cond_t buffer_vazio, buffer_cheio;

//inicializa o buffer
void IniciaBuffer(int n) {
  int i;
  for(i=0; i<n; i++)
    Buffer[i] = 0;
}

//imprime o buffer
void ImprimeBuffer(int n) {
  int i;
  for(i=0; i<n; i++)
    printf("%d ", Buffer[i]);
  printf("\n");
}


void Insere (int id){
    pthread_mutex_lock(&mutex);
    int i;

    printf("P[%d] quer inserir\n", id);

    while(count != 0){
        printf("P[%d] bloqueou\n", id);
        pthread_cond_wait(&buffer_vazio, &mutex);
        printf("P[%d] desbloqueou\n", id);
    }

    for (i = 0; i < N; i++){
        Buffer[i] = rand() % 10;
    }

    count = N;
    printf("P[%d] inseriu\n", id);
    ImprimeBuffer(N);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&buffer_cheio);
}


int Retira(int id){
    int item;

    pthread_mutex_lock(&mutex);
    printf("C[%d] quer consumir\n", id);

    while(count == 0){
        printf("C[%d] bloqueou\n", id);
        pthread_cond_wait(&buffer_cheio, &mutex);
        printf("C[%d] desbloqueou\n", id);
    }

    item = Buffer[count - 1];
    Buffer[count - 1] = 0;
    count--;
    printf("C[%d] consumiu %d\n", id, item);

    if(count == 0){
        pthread_cond_signal(&buffer_vazio);
    }
    
    ImprimeBuffer(N);
    pthread_mutex_unlock(&mutex);

    return item;
}


void *produtor(void*arg){
    int *id = (int *) arg;
    printf("Sou a thread produtora %d\n", *id);

    while(1){
        Insere(*id);
        sleep(1);
    }

    free(arg);
    pthread_exit(NULL);
}

void *consumidor(void *arg){
    int *id = (int *) arg;

    int item;
    printf("Sou a thread consumidora %d\n", *id);
    while(1){
        item = Retira(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

int main(void){

    //variáveis auxialires
    int i;

    //identificadores das threads
    pthread_t tid[P+C];
    int *id[P+C];

    //aloca espaço para os IDs das threads
    for(i = 0; i < P+C; i++){
        id[i] = malloc(sizeof(int));
        if( id[i] == NULL){
            printf("ERRO DE MALLOC\n");
            exit(-1); 
        }
        *id[i] = i+1; 
    }
    IniciaBuffer(N);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&buffer_cheio, NULL);
    pthread_cond_init(&buffer_vazio, NULL);

    //cria as threads produtoras
    for(i=0; i<P; i++) {
        if(pthread_create(&tid[i], NULL, produtor, (void *) id[i])) exit(-1);
    } 

    //cria as threads consumidoras
    for(i=0; i<C; i++) {
        if(pthread_create(&tid[i+P], NULL, consumidor, (void *) id[i+P])) exit(-1);
    } 

    pthread_exit(NULL);

    return 0;
}

