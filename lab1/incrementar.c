#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define USE_THREADS  // Descomente esta linha para usar threads

void preencherVetor(int *vetor, int n) {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    for (int i = 0; i < n; i++) {
        vetor[i] = rand() % n;
    }
}

void imprimirVetor(int *vetor, int tamanho) {
    printf("\nVetor Atual: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

#ifdef USE_THREADS
#include <pthread.h>

#define NTHREADS 4

// Estrutura para passar parâmetros para a thread
typedef struct {
    int id;
    int nthreads;
    int *vetor;
    int tamanho;
    int start;
    int end;
} t_args;

// Função executada pela thread
void *incrementar(void *arg) {
    t_args *args = (t_args*) arg;
    printf("Sou a thread %d de %d threads.\n", args->id, args->nthreads);
    
    for (int i = args->start; i < args->end; i++) {
        args->vetor[i]++;
    }
    
    pthread_exit(NULL);
}

#endif

int main(int argc, char *argv[]) {
    int tamanho;
    int *vetor;

    // Variáveis para medir o tempo de execução
    clock_t start_time, end_time;
    double time_taken;

    if (argc < 2) {
        printf("ERRO de entrada de parametros: Digite %s <tamanho do vetor>\n", argv[0]);
        return 1;
    }

    tamanho = atoi(argv[1]);

    vetor = (int*) malloc(sizeof(int) * tamanho);
    if (vetor == NULL) {
        printf("ERRO de alocação de memória\n");
        return 2;
    }

    preencherVetor(vetor, tamanho);
    imprimirVetor(vetor, tamanho);

    // Captura o tempo de início
    start_time = clock();

#ifdef USE_THREADS
    // Operação de incremento com threads
    pthread_t tid[NTHREADS];

    for (int i = 0; i < NTHREADS; i++) {
        t_args *args = (t_args*) malloc(sizeof(t_args));
        if (args == NULL) {
            printf("ERRO de alocação de memória\n");
            return 2;
        }

        args->id = i;
        args->nthreads = NTHREADS;
        args->vetor = vetor;
        args->tamanho = tamanho;

        // Divisão do vetor entre as threads
        args->start = i * (tamanho / NTHREADS);
        args->end = (i + 1) * (tamanho / NTHREADS);
        if (i == NTHREADS - 1) {
            args->end = tamanho; // A última thread pega o restante do vetor
        }

        // Criação da thread
        if (pthread_create(&tid[i], NULL, incrementar, (void*)args)) {
            printf("ERRO ao criar a thread\n");
            return 3;
        }
    }

    // Aguardar todas as threads terminarem
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

#else
    // Operação de incremento sem threads
    for (int i = 0; i < tamanho; i++) {
        vetor[i]++;
    }
#endif

    // Captura o tempo de término
    end_time = clock();

    imprimirVetor(vetor, tamanho);

    // Calcula o tempo de execução
    time_taken = ((double)end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nTempo de execução: %.4f segundos\n", time_taken);

    // Liberar memória
    free(vetor);
    
    return 0;
}
