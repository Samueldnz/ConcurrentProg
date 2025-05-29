/* Disciplina: Programacao Concorrente */
/* Autor: Samuel Sampaio Diniz */
/* Atividade 6: Produtor/Consumidor com verificação de primalidade */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>

#define PRODUTORES 1 // Número fixo de threads produtoras

int *M;            // Buffer compartilhado (canal de inteiros)
int N;             // Quantidade de inteiros a produzir
int tamBuffer;     // Tamanho do buffer (M)
int C;             // Quantidade de threads consumidoras

// Semáforos para controle de sincronização
sem_t slotCheio, slotVazio;  // Condição: slots ocupados/vazios
sem_t mutex;                 // Exclusão mútua no buffer

// Struct para retorno da thread consumidora
typedef struct {
    int idThread;
    int contadorDePrimos;
} t_Ret;

// Inicializa os semáforos de sincronização
void init_sincronizadores(int tam) {
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, tam);
    sem_init(&mutex, 0, 1);
}

// Verifica se um número é primo
int ehPrimo(long long int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2)
        if (n % i == 0) return 0;
    return 1;
}

// Inicializa o buffer com zeros
void init_canal(int *M, int tam){
    for(int i = 0; i < tam; i++){
        M[i] = 0;
    }
}

// Exibe o conteúdo atual do buffer
void printBuffer(int *buffer, int tam) {
    for(int i = 0; i < tam; i++) 
        printf("%d ", buffer[i]); 
    puts("");
}

// Insere um item no buffer (com sincronização)
void Insere(int item, int id){
    static int in = 0; // posição de escrita

    sem_wait(&slotVazio); // aguarda espaço vazio
    sem_wait(&mutex);     // acesso exclusivo

    M[in] = item;
    in = (in + 1) % tamBuffer;

    printf("Prod[%d]: inseriu %d\n", id, item);
    printBuffer(M, tamBuffer);

    sem_post(&mutex);      // libera acesso
    sem_post(&slotCheio);  // sinaliza novo item
}

// Retira um item do buffer (com sincronização)
int Retira(int id){
    static int out = 0; // posição de leitura
    int item;

    sem_wait(&slotCheio); // aguarda item disponível
    sem_wait(&mutex);     // acesso exclusivo

    item = M[out];
    M[out] = 0;
    out = (out + 1) % tamBuffer;

    printf("Cons[%d]: retirou %d\n", id, item);
    printBuffer(M, tamBuffer);

    sem_post(&mutex);     // libera acesso
    sem_post(&slotVazio); // sinaliza espaço livre

    return item;
}

// Função executada pelas threads consumidoras
void *consumidor(void *arg){
    int item;
    int id = *(int*) (arg);
    free(arg);

    t_Ret *ret = malloc(sizeof(t_Ret));
    if (ret == NULL){
        printf("ERRO de alocação de memória\n");
        exit(1);
    }

    ret->idThread = id;
    ret->contadorDePrimos = 0;

    // Consome até receber -1 (sentinela de término)
    while(1){
        item = Retira(id);
        if (item == -1)
            break;
        
        if(ehPrimo(item)){
            ret->contadorDePrimos++;
        }
    }

    pthread_exit((void*) ret);  
}

// Função executada pela thread produtora
void *produtor(void *arg){
    int id = *(int *) (arg);
    free(arg);
    int i, j;

    for (i = 0; i < N; i++){
        sleep(1); // atraso artificial (opcional)
        Insere(i + 1, id);
    }

    // Envia sentinela (-1) para cada thread consumidora
    for (j = 0; j < C; j++) {
        Insere(-1, id);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    t_Ret *retorno;
    int i, *id;
    int IdVencedora;
    int maior = 0;

    // Validação dos argumentos
    if (argc < 4){
        printf("ERRO de entrada de parametros: Digite %s <N> <tamBuffer> <C>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    tamBuffer = atoi(argv[2]);
    C = atoi(argv[3]);

    // Alocação do buffer
    M = (int*) malloc(sizeof(int) * tamBuffer);
    if(M == NULL){
        printf("ERRO de alocação de memória\n");
        return 2;
    }

    init_canal(M, tamBuffer);
    init_sincronizadores(tamBuffer);

    pthread_t tid[C + PRODUTORES]; // threads produtoras + consumidoras

    // Criação da thread produtora
    for (i = 0; i < PRODUTORES; i++){
        id = (int*) malloc(sizeof(int));
        *id = i + 1;

        if(pthread_create(&tid[i], NULL, produtor, (void*) id)){
            printf("Erro na criação da thread produtora\n");
            return 3;
        }
    }

    // Criação das threads consumidoras
    for (i = 0; i < C; i++){
        id = (int*) malloc(sizeof(int));
        *id = i + 1;

        if(pthread_create(&tid[PRODUTORES + i], NULL, consumidor, (void*) id)){
            printf("Erro na criação da thread consumidora\n");
            return 4;
        }
    }

    // Espera o término das threads consumidoras
    for (i = 0; i < C; i++){
        if(pthread_join(tid[PRODUTORES + i], (void**) &retorno)){
            printf("--ERRO: pthread_join() da thread consumidora %d\n", i);
            return 5;
        }

        printf("Thread consumidora %d encontrou %d primos\n", retorno->idThread, retorno->contadorDePrimos);

        // Verifica se é a vencedora
        if(retorno->contadorDePrimos > maior){
            IdVencedora = retorno->idThread;
            maior = retorno->contadorDePrimos;
        }

        free(retorno);
    }

    printf("\nA Vencedora foi a Thread consumidora %d\n", IdVencedora);
    printf("--Thread principal terminou\n");

    return 0;
}
