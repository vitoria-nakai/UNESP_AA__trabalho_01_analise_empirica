#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>

#include "ordenacao.c"


// Função auxiliar para copiar vetores antes de ordenar
void copiarVetor(const int *origem, int *destino, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        destino[i] = origem[i];
    }
}

int carregarInstancia(const char *caminho, int **vetor){
    
    //ABERTURA DO ARQUIVO
    FILE * f = fopen(caminho, "r");
    if(f == NULL){
        printf("Erro ao abri o arquivo %s\n", caminho);
        return -1;
    }

    //GUARDANDO O TAMANHO DO ARQUIVO NA VARIÁVEL n
    int n;
    if(fscanf(f, "%d", &n) != 1){
        printf("Erro ao ler o tamanho N no arquivo %s\n", caminho);
        fclose(f);
        return -1;
    }

    //ALOCANDO MEMÓRIA AO VETOR
    *vetor = (int*)malloc(n * sizeof(int));
    if(*vetor == NULL){
        printf("Erro de memória para N = %d\n", n);
        fclose(f);
        return -1;
    }

    //GUARDANDO OS ELEMENTOS DO ARQUIVO NO VETOR
    for(int i = 0; i < n; i++){
        if(fscanf(f, "%d", &(*vetor)[i]) != 1){
            printf("Erro ao ler elemento %d no arquivo %s\n", i, caminho);
            free(*vetor);
            fclose(f);
            return -1;
        }
    }

    fclose(f);
    return n;
}

void testarEGravar(FILE *csv, const char *nomeAlg, void(*func)(int*, int, Metricas*), const int *origem, int *buffer, int n, const char *cenario, int rep){

    Metricas m;
    copiarVetor(origem, buffer, n);
    func(buffer, n, &m);

    //EXIBINDO NO CONSOLE
    printf("%-18s %-12s %-8d %-5d %-15lld %-15lld %.6fs\n", nomeAlg, cenario, n, rep, m.comparacoes, m.trocas, m.tempo);


    //GRAVANDO NO CSV com separador ponto e vírgula
    if(csv != NULL){
        fprintf(csv, "%s;%s;%d;%d;%lld;%lld;%.6f\n", nomeAlg, cenario, n, rep, m.comparacoes, m.trocas, m.tempo);
    }
}

void executarTodosAlgoritmos(FILE *csv, const int *origem, int *buffer, int n, const char *cenario, int rep){
    testarEGravar(csv, "Selection Sort", selectionSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Insertion Sort", insertionSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Merge Sort", mergeSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Quick Sort", quickSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Heap Sort", heapSort, origem, buffer, n, cenario, rep);

}

int main () 
{
    int tamanhos[] = {1000, 5000, 10000, 20000, 40000};
    int total_tamanhos = 5;
    char caminho[128];

    // Abertura do arquivo CSV para escrita
    FILE *arquivoCsv = fopen("metricas.csv", "w");
    if (arquivoCsv == NULL) {
        printf("Aviso: Não foi possível criar o arquivo metricas.csv\n\n");
        return 1;
    }

    //GUARDANDO O CABEÇALHO PADRÃO
    fprintf(arquivoCsv, "Algoritmo;Cenario;N;Repeticao;Comparacoes;Trocas;Tempo_s\n");
    printf("%-18s %-12s %-8s %-5s %-15s %-15s %-10s\n", "Algoritmo", "Cenario", "N", "Rep", "Comparacoes", "Trocas", "Tempo(s)");
    printf("----------------------------------------------------------------------------------------\n");

    for(int t = 0; t < total_tamanhos; t++){
        int n = tamanhos[t];
        

        //DADOS ALEATÓRIOS
        for(int rep = 1; rep <= 10; rep++){
            sprintf(caminho, "aleatorio_%d_%d.txt", n, rep);

            int *vetorOrigem = NULL;
            if(carregarInstancia(caminho, &vetorOrigem) < 0) continue;

            int *bufferTeste = (int*)malloc(n * sizeof(int));
            executarTodosAlgoritmos(arquivoCsv, vetorOrigem, bufferTeste, n, "Aleatorio", rep);

            free(vetorOrigem);
            free(bufferTeste);
        }

        //DADOS ORDENADOS DE FORMA CRESCENTE
        sprintf(caminho, "Ordenado_%d.txt", n);
        int *vetorOrdenado = NULL;
        if (carregarInstancia(caminho, &vetorOrdenado) >= 0){
            int *bufferTeste = (int*)malloc(n * sizeof(int));
            for(int rep = 1; rep <= 10; rep++){
                executarTodosAlgoritmos(arquivoCsv, vetorOrdenado, bufferTeste, n, "Ordenado", rep);
            }
            free(vetorOrdenado);
            free(bufferTeste);
        }

        //DADOS ORDENADOS DE FORMA DECRESCENTE
        sprintf(caminho, "invertido_%d.txt", n);
        int *vetorInvertido = NULL;
        if(carregarInstancia(caminho, &vetorInvertido) >= 0){
            int *bufferTeste = (int*)malloc(n*sizeof(int));
            for(int rep = 1; rep <= 10; rep++){
                executarTodosAlgoritmos(arquivoCsv, vetorInvertido, bufferTeste, n, "Invertido", rep);    
            }
            free(vetorInvertido);
            free(bufferTeste);
        }
    }
    fclose(arquivoCsv);
    printf("\nBateria de testes finalizada com sucesso! Dados exportados para 'metricas.csv'.\n");
    return 0;
}