#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

#define MAX 1000   // Tamanho máximo do vetor
#define TEXTO      // Define que os vetores serão impressos na saída padrão

long int dimensao; //dimensao dos vetores
float *vetor1;
float *vetor2;
int nthreads;
float resultado_seq; //resultado do programa sequencial
float resultado_final; //resultado do programa concorrente

typedef struct {
    float *vetor1;
    float *vetor2;
    long int inicio;
    long int fim;
    float resultado_parcial;
} DadosThread;


float variacaoRelativa(){
    return fabs((resultado_seq - resultado_final) / resultado_seq);
}

void *calcularProdutoInterno(void *arg) {
    DadosThread *dados = (DadosThread *)arg;
    dados->resultado_parcial = 0.0;

    for (long int i = dados->inicio; i < dados->fim; i++) {
        dados->resultado_parcial += dados->vetor1[i] * dados->vetor2[i];
    }

    pthread_exit(NULL);
}

float produtoInterno(float *vetor1, float *vetor2) {
    pthread_t threads[nthreads];
    DadosThread dados[nthreads];
    long int intervalo = dimensao / nthreads;
    float resultado_final = 0.0;

    // Cria as threads
    for (int i = 0; i < nthreads; i++) {
        dados[i].vetor1 = vetor1;
        dados[i].vetor2 = vetor2;
        dados[i].inicio = i * intervalo;
        dados[i].fim = (i == nthreads - 1) ? dimensao : (i + 1) * intervalo;  // Última thread pega o restante
        dados[i].resultado_parcial = 0.0;
        
        pthread_create(&threads[i], NULL, calcularProdutoInterno, (void *)&dados[i]);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
        resultado_final += dados[i].resultado_parcial;
    }

    return resultado_final;
}

int main(int argc, char *argv[]) {
    double variacao;
    FILE *arq; //arquivo de entrada
    size_t ret; //retorno da função de leitura do arquivo de entrada

    if (argc < 3) {
        fprintf(stderr, "Digite: %s <número de threads> <nome arquivo entrada>\n", argv[0]);
        return 1;
    }

    //abre o arquivo de entrada com os valores para serem somados
    arq = fopen(argv[2], "rb");
    if(arq==NULL) { printf("--ERRO: fopen()\n"); exit(-1); }

    //le o tamanho do vetor (primeira linha do arquivo)
    ret = fread(&dimensao, sizeof(long int), 1, arq);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }

    vetor1 = (float*) malloc(sizeof(float) * dimensao);
    if(vetor1 == NULL) {printf("---ERRO: malloc()\n"); exit(1);}
    ret = fread(vetor1, sizeof(float), dimensao, arq);
    if(ret < dimensao){
        fprintf(stderr, "Erro de leitura dos elemento do vetor\n");
        return 4;
    }

    vetor2 = (float*) malloc(sizeof(float) * dimensao);
    if(vetor2 == NULL) {printf("---ERRO: malloc()\n"); exit(1);}
    ret = fread(vetor2, sizeof(float), dimensao, arq);
    if(ret < dimensao){
        fprintf(stderr, "Erro de leitura dos elemntos do vetor\n");
        return 4;
    }

    ret = fread(&resultado_seq, sizeof(float), 1, arq);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }

    nthreads = atoi(argv[1]);
    if(nthreads > dimensao) nthreads = dimensao;

    resultado_final = produtoInterno(vetor1, vetor2);

    #ifdef TEXTO
    fprintf(stdout, "%ld\n", dimensao);
    fprintf(stdout, "Vetor1: ");
    for(long int i=0; i<dimensao; i++) {
        fprintf(stdout, "%f ",vetor1[i]);
    }
    fprintf(stdout, "\n");
    #endif

        //imprimir na saida padrao o vetor gerado
    #ifdef TEXTO
    fprintf(stdout, "%ld\n", dimensao);
    fprintf(stdout, "Vetor2: ");
    for(long int i=0; i<dimensao; i++) {
        fprintf(stdout, "%f ",vetor2[i]);
    }
    fprintf(stdout, "\n");
    #endif

    if(resultado_final > resultado_seq){
        printf("Produto interno do programa concorrente maior!  %f > %f\n", resultado_final, resultado_seq);
    }else if(resultado_final < resultado_seq){
        printf("Produto interno do programa sequencial maior!  %f > %f\n", resultado_seq, resultado_final);
    }else{
        printf("Produto interno igual nos dois programas!  %f = %f\n", resultado_final, resultado_seq);
    }

    variacao = variacaoRelativa();
    printf("Variação relativa: %.16f\n", variacao);

    return 0;
}
