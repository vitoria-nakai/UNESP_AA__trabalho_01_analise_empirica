#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ordenacao.c"
#include "gerador-vetores.c"

// DEFINIR TAMANHO DO VETOR
#define TAM 5

// Função auxiliar para copiar vetores antes de ordenar
void copiarVetor(int *origem, int *destino, int tam) {
    for (int i = 0; i < tam; i++) {
        destino[i] = origem[i];
    }
}

// Executa um algoritmo para os 3 cenários, imprime a tabela no terminal e grava no CSV
void testarAlgoritmo(FILE *arquivoCsv, const char *nomeAlgoritmo, void (*funcOrdenacao)(int*, int, Metricas*), int tamanho) {
    int *vetorOrigem = (int*) malloc(tamanho * sizeof(int));
    int *vetorTeste = (int*) malloc(tamanho * sizeof(int));
    Metricas m;

    const char *tiposVetor[] = {"Ordenado", "Aleatorio", "Invertido"};

    for (int i = 0; i < 3; i++) {
        // Gerar o vetor conforme o cenário
        if (i == 0) geraOrdenado(vetorOrigem, tamanho);
        else if (i == 1) geraAleatorio(vetorOrigem, tamanho);
        else geraInvertido(vetorOrigem, tamanho);

        // Copiar o vetor para não alterar a fonte original
        copiarVetor(vetorOrigem, vetorTeste, tamanho);

        // Executar a ordenação
        funcOrdenacao(vetorTeste, tamanho, &m);

        // Imprimir linha formatada na tela
        printf("%-20s %-15s %-15lld %-15lld %.6fs\n", 
               nomeAlgoritmo, tiposVetor[i], m.comparacoes, m.trocas, m.tempo);

        // Escrever a linha no arquivo CSV (usando ponto e vírgula como separador)
        if (arquivoCsv != NULL) {
            fprintf(arquivoCsv, "%s;%s;%lld;%lld;%.6f\n", 
                    nomeAlgoritmo, tiposVetor[i], m.comparacoes, m.trocas, m.tempo);
        }
    }

    free(vetorOrigem);
    free(vetorTeste);
}

int main () 
{
    int tamanho = TAM;
    srand(time(NULL));

    // Abertura do arquivo CSV para escrita
    FILE *arquivoCsv = fopen("metricas.csv", "w");
    if (arquivoCsv == NULL) {
        printf("Aviso: Não foi possível criar o arquivo metricas.csv\n\n");
    } else {
        // Escreve o cabeçalho no CSV
        fprintf(arquivoCsv, "Algoritmo;Vetor;Comparacoes;Trocas;Tempo\n");
    }

    // Exibe o cabeçalho no console
    printf("%-20s %-15s %-15s %-15s %-10s\n", "Algoritmo", "Vetor", "Comparacoes", "Trocas", "Tempo");
    printf("-----------------------------------------------------------------------------------\n");

    // Executa e registra todos os algoritmos
    testarAlgoritmo(arquivoCsv, "Selection Sort", selectionSort, tamanho);
    testarAlgoritmo(arquivoCsv, "Insertion Sort", insertionSort, tamanho);
    testarAlgoritmo(arquivoCsv, "Merge Sort", mergeSort, tamanho);
    testarAlgoritmo(arquivoCsv, "Quick Sort", quickSort, tamanho);
    testarAlgoritmo(arquivoCsv, "Heap Sort", heapSort, tamanho);

    // Fecha o arquivo caso tenha sido aberto com sucesso
    if (arquivoCsv != NULL) {
        fclose(arquivoCsv);
        printf("\nResultados salvos com sucesso no arquivo 'metricas.csv'!\n");
    }

    return 0;
}
