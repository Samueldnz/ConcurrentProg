#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int N;
long long int current = 1; //número atual a ser testado
int totalPrimos = 0; //variável a ser acrescentada
pthread_mutex_t mutex; //variavel de lock para exclusão mútua

/*função de verificação de número primo*/
int ehPrimo(long long int n){
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n%2 == 0) return 0;
    for (i = 3; i < sqrt(n) + 1; i+=2){
        if(n%i==0) return 0;
    }
    return 1;
}

/*função da thread*/
void *Func(void* arg){
    long long int num;

    while (1) {
        pthread_mutex_lock(&mutex);
        if (current > N) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        num = current;
        current++;
        pthread_mutex_unlock(&mutex);

        if (ehPrimo(num)) {
            pthread_mutex_lock(&mutex);
            totalPrimos++;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

/*função para ler o tempo T(1) salvo anteriormente*/
double ler_tempo_base(){
    FILE* f = fopen("tempo_base.txt", "r");
    if (!f) {
        fprintf(stderr, "Aviso: tempo_base.txt não encontrado.\n");
        return -1; // indica que não foi lido com sucesso
    }

    double t;
    if (fscanf(f, "%lf", &t) != 1) {
        fprintf(stderr, "Erro ao ler tempo base do arquivo.\n");
        fclose(f);
        return -1;
    }

    fclose(f);
    return t;
}

/*função para salvar o tempo T(1)*/
void salvar_tempo_base(double t){
    FILE* f = fopen("tempo_base.txt", "w");
    if (!f) {
        fprintf(stderr, "Erro ao abrir tempo_base.txt para escrita.\n");
        return;
    }

    fprintf(f, "%.10lf\n", t); // salva com alta precisão
    fclose(f);
}


/*fluxo principal*/
int main(int argc, char *argv[]){
    pthread_t *tid; //identificadores das threads no sistema
    int nthreads;
    double inicio, fim, total; //variáveis do cálculo do tempo

    if(argc < 3){
        printf("Digite: %s <Valor N> <Número de threads>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    
    GET_TIME(inicio);
    tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
    if(tid == NULL) {puts("ERRO --malloc"); return 2;}

    pthread_mutex_init(&mutex, NULL);

    for (long int t = 0; t < nthreads; t++){
        if (pthread_create(&tid[t], NULL, Func, NULL)){
            printf("--ERRO: pthread_create()\n"); exit(1);
        }
    }

    for (int t = 0; t < nthreads; t++){
        if (pthread_join(tid[t], NULL)){
            printf("--ERRO: pthread_join()\n"); exit(1);
        }
    }

    GET_TIME(fim);
    total = fim - inicio;

    double tempo_base;
    
    if(nthreads == 1){
        salvar_tempo_base(total);
        tempo_base = total;
    }else{
        tempo_base = ler_tempo_base();
    }

    if (tempo_base <= 0) {
        printf("Tempo base inválido. Aceleração e eficiência não podem ser calculadas.\n");
    } else {
        double aceleracao = tempo_base / total;
        double eficiencia = aceleracao / nthreads;

        printf("%lld, %d, %d, %.6lf, %.6lf, %.6lf\n", N, nthreads, totalPrimos, total, aceleracao, eficiencia);
    }

    pthread_mutex_destroy(&mutex);
    free(tid);

    return 0;
}