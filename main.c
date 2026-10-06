#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ordenacao.c"

// Copia o vetor para não alterar a fonte original entre os diferentes algoritmos
void copiarVetor(const int *origem, int *destino, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        destino[i] = origem[i];
    }
}

// Carrega o vetor a partir do arquivo .txt gerado
int carregarInstancia(const char *caminho, int **vetor) {
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        printf("Aviso: Nao foi possivel abrir o arquivo %s\n", caminho);
        return -1;
    }

    int n;
    if (fscanf(f, "%d", &n) != 1) {
        printf("Erro ao ler o tamanho N no arquivo %s\n", caminho);
        fclose(f);
        return -1;
    }

    *vetor = (int*) malloc(n * sizeof(int));
    if (*vetor == NULL) {
        printf("Erro de memoria para N = %d\n", n);
        fclose(f);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%d", &(*vetor)[i]) != 1) {
            printf("Erro ao ler elemento %d no arquivo %s\n", i, caminho);
            free(*vetor);
            fclose(f);
            return -1;
        }
    }

    fclose(f);
    return n;
}

// Executa um algoritmo sobre o vetor carregado e grava a linha no CSV
void testarEGravar(FILE *csv, const char *nomeAlg, void (*func)(int*, int, Metricas*),
                   const int *origem, int *buffer, int n, const char *cenario, int rep) {
    Metricas m;
    copiarVetor(origem, buffer, n);
    func(buffer, n, &m);

    // Exibe no console formatado
    printf("%-18s %-12s %-8d %-5d %-15lld %-15lld %.6fs\n",
           nomeAlg, cenario, n, rep, m.comparacoes, m.trocas, m.tempo);

    // Grava no CSV com separador ponto e vírgula
    if (csv != NULL) {
        fprintf(csv, "%s;%s;%d;%d;%lld;%lld;%.6f\n",
                nomeAlg, cenario, n, rep, m.comparacoes, m.trocas, m.tempo);
    }
}

// Executa os 5 algoritmos implementados pela Pessoa 1
void executarTodosAlgoritmos(FILE *csv, const int *origem, int *buffer, int n, const char *cenario, int rep) {
    testarEGravar(csv, "Selection Sort", selectionSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Insertion Sort", insertionSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Merge Sort", mergeSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Quick Sort", quickSort, origem, buffer, n, cenario, rep);
    testarEGravar(csv, "Heap Sort", heapSort, origem, buffer, n, cenario, rep);
}

int main() {
    int tamanhos[] = {1000, 5000, 10000, 20000, 40000};
    int total_tamanhos = 5;
    char caminho[128];

    FILE *arquivoCsv = fopen("metricas.csv", "w");
    if (arquivoCsv == NULL) {
        printf("Erro critico: Nao foi possivel criar metricas.csv\n");
        return 1;
    }

    // Cabecalho padronizado para as analises e graficos
    fprintf(arquivoCsv, "Algoritmo;Cenario;N;Repeticao;Comparacoes;Trocas;Tempo_s\n");

    printf("%-18s %-12s %-8s %-5s %-15s %-15s %-10s\n",
           "Algoritmo", "Cenario", "N", "Rep", "Comparacoes", "Trocas", "Tempo(s)");
    printf("----------------------------------------------------------------------------------------\n");

    for (int t = 0; t < total_tamanhos; t++) {
        int n = tamanhos[t];

        // 1. Cenário Aleatório (10 arquivos distintos por tamanho)
        for (int rep = 1; rep <= 10; rep++) {
            // Se os arquivos estiverem numa subpasta, altere para "instancias/aleatorio_%d_%d.txt"
            sprintf(caminho, "../dados_entrada/aleatorio_%d_%d.txt", n, rep);

            int *vetorOrigem = NULL;
            if (carregarInstancia(caminho, &vetorOrigem) < 0) continue;

            int *bufferTeste = (int*) malloc(n * sizeof(int));
            executarTodosAlgoritmos(arquivoCsv, vetorOrigem, bufferTeste, n, "Aleatorio", rep);

            free(vetorOrigem);
            free(bufferTeste);
        }

        // 2. Cenário Ordenado (1 arquivo lido e avaliado 10 vezes para estabilidade de tempo)
        sprintf(caminho, "../dados_entrada/ordenado_%d.txt", n);
        int *vetorOrdenado = NULL;
        if (carregarInstancia(caminho, &vetorOrdenado) >= 0) {
            int *bufferTeste = (int*) malloc(n * sizeof(int));
            for (int rep = 1; rep <= 10; rep++) {
                executarTodosAlgoritmos(arquivoCsv, vetorOrdenado, bufferTeste, n, "Ordenado", rep);
            }
            free(vetorOrdenado);
            free(bufferTeste);
        }

        // 3. Cenário Invertido (1 arquivo lido e avaliado 10 vezes)
        sprintf(caminho, "../dados_entrada/invertido_%d.txt", n);
        int *vetorInvertido = NULL;
        if (carregarInstancia(caminho, &vetorInvertido) >= 0) {
            int *bufferTeste = (int*) malloc(n * sizeof(int));
            for (int rep = 1; rep <= 10; rep++) {
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