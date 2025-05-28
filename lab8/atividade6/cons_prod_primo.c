/* Disciplina: Programacao Concorrente */
/* Autor: Samuel Sampaio Diniz */


#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>

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


int main(int argc, char* argv[]){

    int N, tamCanal, C;
    int *M;

    if (argc < 4){
        printf("ERRO de entrada de parametros: Digite %s <quantidade de inteiros a produzir> <tamanho do canal (pequeno)> <número de threads consumidoras>\n", argv[0]);
        return 1;
    }

    tamCanal = atoi(argv[2]);
    M = (int*) malloc(sizeof(int) * tamCanal);
    if(M == NULL){
        printf("ERRO de alocação de memória\n");
        return 2;
    }
    init_canal(M, tamCanal);

    N = atoi(argv[1]);
    C = atoi(argv[3]);


    return 0;
}

