#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void geraAleatorio(int *A, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        A[i] = rand() % 100;
    }
}

void geraOrdenado(int *A, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        A[i] = i;
    }
}

void geraInvertido(int *A, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        A[i] = tam - i - 1;
    }
}

void imprimirVetor(int *A, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        if (i == 0) printf("[");
        if (i < tam - 1) printf("%d, ", A[i]);
        else printf("%d]", A[i]);
    }
}

/*
int main()
{
    int tamanho = 10;

    int *vetor = (int*) malloc(tamanho * sizeof(int));

    if (vetor == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    srand(time(NULL));

    // Vetor aleatório
    geraAleatorio(vetor, tamanho);

    printf("Vetor aleatorio: ");

    for (int i = 0; i < tamanho; i++)
    {
        printf("%d ", vetor[i]);
    }

    // Vetor ordenado
    geraOrdenado(vetor, tamanho);

    printf("\nVetor ordenado: ");

    for (int i = 0; i < tamanho; i++)
    {
        printf("%d ", vetor[i]);
    }

    // Vetor invertido
    geraInvertido(vetor, tamanho);

    printf("\nVetor invertido: ");

    for (int i = 0; i < tamanho; i++)
    {
        printf("%d ", vetor[i]);
    }

    free(vetor);

    return 0;
}
*/
