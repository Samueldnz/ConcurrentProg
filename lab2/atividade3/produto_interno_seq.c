/*
Programa auxiliar para gerar dois vetores de floats para o cálculo de produto interno
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 1000 // Tamanho máximo do vetor
#define TEXTO    // Define que os vetores serão impressos na saída padrão

double fator = 1.0;

void preencherVetor(float *vetor1, float *vetor2, int n) {
    // Garante que a função rand() tenha um valor de semente diferente a cada execução
    srand(time(NULL));
    
    for(int i = 0; i < n; i++) {
        // Gera um número aleatório do tipo float, por exemplo, entre 0 e 1000
        vetor1[i] = ((rand() % n) / 3.0) * fator;  // Isso gera números entre 0.0 e 333.33

        fator *=-1;
    }

    for(int i = 0; i < n; i++){
        vetor2[i] = ((rand() % n) / 3.0)*fator;
        fator *=-1;
    }
}

float produtoInterno(float *vetor1, float *vetor2, int n) {
    float produtoInterno = 0.0;
    for(int i = 0; i < n; i++) {
        produtoInterno += vetor1[i] * vetor2[i]; // Calcula o produto interno entre os vetores
    }
    return produtoInterno;
}

int main(int argc, char *argv[]){
    float *vetor1; // Vetor 1 que será gerado
    float *vetor2; // Vetor 2 que será gerado
    long int n;    // Quantidade de elementos do vetor
    FILE *descritorArquivo;
    size_t ret;     // Retorno da função de escrita no arquivo de saída

    // Variáveis para medição de tempo
    clock_t start, end;
    double cpu_time_used;

    // Recebe os argumentos de entrada
    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensão> <nome arquivo saída>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]); // Converte o primeiro argumento para o número de elementos do vetor

    // Verifica se a dimensão de entrada é permitida
    if (n > MAX){
        fprintf(stderr, "Dimensão máxima permitida: %d\n", MAX);
        return 4;
    }

    // Aloca memória para os vetores
    vetor1 = (float*) malloc(sizeof(float) * n);
    if(!vetor1){
        fprintf(stderr, "Erro de alocação de memória do vetor 1\n");
        return 2;
    }

    vetor2 = (float*) malloc(sizeof(float) * n);
    if(!vetor2){
        fprintf(stderr, "Erro de alocação de memória do vetor 2\n");
        return 2;
    }

    preencherVetor(vetor1, vetor2, n);

        //imprimir na saida padrao o vetor gerado
    #ifdef TEXTO
    fprintf(stdout, "%ld\n", n);
    fprintf(stdout, "Vetor1: ");
    for(long int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vetor1[i]);
    }
    fprintf(stdout, "\n");
    #endif

        //imprimir na saida padrao o vetor gerado
    #ifdef TEXTO
    fprintf(stdout, "%ld\n", n);
    fprintf(stdout, "Vetor2: ");
    for(long int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vetor2[i]);
    }
    fprintf(stdout, "\n");
    #endif

    // Abre o arquivo para escrita binária
    descritorArquivo = fopen(argv[2], "wb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do arquivo.\n");
        return 3;
    }

    // Escreve o tamanho do vetor no arquivo
    ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
    // Escreve o vetor 1 no arquivo
    ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);
    if(ret < n) {
        fprintf(stderr, "Erro 1 de escrita no arquivo\n");
        return 4;
    }

    // Escreve o vetor 2 no arquivo
    ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);
    if(ret < n){
        fprintf(stderr, "Erro 2 de escrita no arquivo\n");
        return 4;
    }

    // Medir o tempo para calcular o produto interno
    start = clock();
    float pI = produtoInterno(vetor1, vetor2, n);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tempo para calcular o produto interno: %f segundos\n", cpu_time_used);

    // Escreve o produto interno no arquivo
    ret = fwrite(&pI, sizeof(float), 1, descritorArquivo);
    #ifdef TEXTO
    fprintf(stdout, "Produto interno calculado: %f\n", pI);
    #endif

    // Fecha o arquivo e libera a memória alocada
    fclose(descritorArquivo);
    free(vetor1);
    free(vetor2);

    return 0; // Fim do programa
}
