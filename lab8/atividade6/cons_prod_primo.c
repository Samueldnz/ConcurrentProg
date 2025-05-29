/* Disciplina: Programacao Concorrente */
/* Autor: Samuel Sampaio Diniz */


#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>

#define PRODUTORES 1
// #define DEBUG

int *M; //buffer compartilhado com tamanho definido pelo usuário


int ehPrimo(long long int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

void init_canal(int *M, int tam){
    int i;

    for(i = 0; i < tam; i++){
        M[i] = 0;
    }
}

#ifdef DEBUG
    void printBuffer(int *buffer, int tam) {
    for(int i=0;i<tam;i++) 
        printf("%d ", buffer[i]); 
    puts("");
    }
#endif

void *consumidor(void *arg){

    //a definir

    pthread_exit(NULL);
}


void *produtor(void *arg){

    //a definir

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){

    int N; // quantidade de inteiros a serem produzidos
    int tamBuffer; // variável auxiliar par definir o tamanho do buffer M
    int C; // quantidade de threads consumidores

    if (argc < 4){
        printf("ERRO de entrada de parametros: Digite %s <quantidade de inteiros a produzir> <tamanho do canal (pequeno)> <número de threads consumidoras>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    tamBuffer = atoi(argv[2]);
    C = atoi(argv[3]);
    
    M = (int*) malloc(sizeof(int) * tamBuffer);
    if(M == NULL){
        printf("ERRO de alocação de memória\n");
        return 2;
    }
    init_canal(M, tamBuffer);

    #ifdef DEBUG
        printBuffer(M, tamBuffer);
    #endif

    pthread_t tid[C + 1];
    int i, *id;

    for (i = 0; i<PRODUTORES; i++){
        id = (int*) malloc(sizeof(int));
        if(id == NULL){
            printf("ERRO de alocação de memória\n");
            return 2;
        }
        *id = i+1;

        if(pthread_create(&tid[i], NULL, produtor, (void*) (id))){
            printf("Erro na criacao do thread produtor\n");
            return 3;
        }
    }

    for (i = 0; i < C; i++){
        id = (int*) malloc(sizeof(int));
        if(id == NULL){
            printf("ERRO de alocação de memória\n");
            return 2;
        }

        if(pthread_create(&tid[PRODUTORES+i], NULL, consumidor, (void*)(id))){
            printf("Erro na criacao do thread consumidor\n");
            return 4;
        }
    }

    //pthread_exit(NULL);

    //provavelmente vou ter que usar um pthread_join e struct para captar os retornos com os acertos de cada thread em relação ao teste de primaridade
    return 0;
}

