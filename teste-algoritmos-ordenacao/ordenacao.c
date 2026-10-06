#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para armazenar as métricas de ordenação
typedef struct {
    long long comparacoes;
    long long trocas;
    double tempo;
} Metricas;

void resetaMetricas(Metricas *m) {
    if (m != NULL) {
        m->comparacoes = 0;
        m->trocas = 0;
        m->tempo = 0.0;
    }
}

// Selection Sort
void selectionSort(int* A, int tam, Metricas *m) 
{
    resetaMetricas(m);
    clock_t inicio = clock();

    int i, j, menor, aux;
    
    for (i = 0; i < tam - 1; i++)
    {
        menor = i;

        for (j = i + 1; j < tam; j++)
        {
            m->comparacoes++;
            if (A[j] < A[menor])
            {
                menor = j;
            }
        }

        if (menor != i) {
            aux = A[i];
            A[i] = A[menor];
            A[menor] = aux;
            m->trocas++;
        }
    }

    clock_t fim = clock();
    m->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

// Insertion Sort
void insertionSort(int* A, int tam, Metricas *m)
{
    resetaMetricas(m);
    clock_t inicio = clock();

    int i, j, chave;

    for (i = 1; i < tam; i++)
    {
        chave = A[i];
        j = i - 1;

        while (j >= 0)
        {
            m->comparacoes++;
            if (A[j] > chave) {
                A[j + 1] = A[j];
                m->trocas++; // Deslocamento de elemento
                j--;
            } else {
                break;
            }
        }

        A[j + 1] = chave;
    }

    clock_t fim = clock();
    m->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

// Auxiliar Merge
void merge(int *A, int inicio, int meio, int fim, Metricas *m)
{
    int i = inicio;
    int j = meio + 1;
    int k = 0;

    int tamanho = fim - inicio + 1;
    int *aux = (int*) malloc(tamanho * sizeof(int));

    while (i <= meio && j <= fim)
    {
        m->comparacoes++;
        if (A[i] <= A[j])
        {
            aux[k] = A[i];
            i++;
        }
        else
        {
            aux[k] = A[j];
            j++;
        }
        m->trocas++; // Cópia para o vetor auxiliar
        k++;
    }

    while (i <= meio)
    {
        aux[k] = A[i];
        i++;
        k++;
        m->trocas++;
    }

    while (j <= fim)
    {
        aux[k] = A[j];
        j++;
        k++;
        m->trocas++;
    }

    for (i = inicio, k = 0; i <= fim; i++, k++)
    {
        A[i] = aux[k];
        m->trocas++; // Cópia de volta para o vetor original
    }

    free(aux);
}

void mergeSortRecursivo(int *A, int inicio, int fim, Metricas *m)
{
    if (inicio < fim)
    {
        int meio = (inicio + fim) / 2;

        mergeSortRecursivo(A, inicio, meio, m);
        mergeSortRecursivo(A, meio + 1, fim, m);
        merge(A, inicio, meio, fim, m);
    }
}

// Merge Sort (Função Principal com medição de tempo)
void mergeSort(int *A, int tam, Metricas *m)
{
    resetaMetricas(m);
    clock_t inicio = clock();

    mergeSortRecursivo(A, 0, tam - 1, m);

    clock_t fim = clock();
    m->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

// Função auxiliar de troca com contagem
void troca(int *a, int *b, Metricas *m)
{
    int aux = *a;
    *a = *b;
    *b = aux;
    if (m != NULL) m->trocas++;
}

int particiona(int *A, int inicio, int fim, Metricas *m)
{
    int pivo = A[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++)
    {
        m->comparacoes++;
        if (A[j] <= pivo)
        {
            i++;
            troca(&A[i], &A[j], m);
        }
    }

    troca(&A[i + 1], &A[fim], m);

    return i + 1;
}

void quickSortRecursivo(int *A, int inicio, int fim, Metricas *m)
{
    if (inicio < fim)
    {
        int posPivo = particiona(A, inicio, fim, m);

        quickSortRecursivo(A, inicio, posPivo - 1, m);
        quickSortRecursivo(A, posPivo + 1, fim, m);
    }
}

// Quick Sort (Função Principal com medição de tempo)
void quickSort(int *A, int tam, Metricas *m)
{
    resetaMetricas(m);
    clock_t inicio = clock();

    quickSortRecursivo(A, 0, tam - 1, m);

    clock_t fim = clock();
    m->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
}

void heapify(int *A, int tam, int i, Metricas *m)
{
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < tam) {
        m->comparacoes++;
        if (A[esquerda] > A[maior]) {
            maior = esquerda;
        }
    }

    if (direita < tam) {
        m->comparacoes++;
        if (A[direita] > A[maior]) {
            maior = direita;
        }
    }

    if (maior != i)
    {
        troca(&A[i], &A[maior], m);
        heapify(A, tam, maior, m);
    }
}

// Heap Sort (Função Principal com medição de tempo)
void heapSort(int *A, int tam, Metricas *m)
{
    resetaMetricas(m);
    clock_t inicio = clock();

    for (int i = tam / 2 - 1; i >= 0; i--)
    {
        heapify(A, tam, i, m);
    }

    for (int i = tam - 1; i > 0; i--)
    {
        troca(&A[0], &A[i], m);
        heapify(A, i, 0, m);
    }

    clock_t fim = clock();
    m->tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
}