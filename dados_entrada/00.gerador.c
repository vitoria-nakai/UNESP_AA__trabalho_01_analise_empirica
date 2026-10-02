#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void gerar_aleatorio(int *vetor, int n){
    for(int i = 0; i < n; i++){
        vetor[i] = rand() % 1000001;
    }
}

void gerar_ordenado(int *vetor, int n){
    for(int i = 0; i < n; i++){
        vetor[i] = i + 1; 
    }
}

void gerar_invertido(int *vetor, int n){
    for(int i = 0; i < n; i++){
        vetor[i] = n - i;
    }
}

void salvar_em_arquivo(const char *nome_arquivo, int *vetor, int n){
    FILE * arq = fopen(nome_arquivo, "w");
    if(arq == NULL){
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return;
    }
    if(fprintf(arq, "%d\n", n) < 0){
        printf("Erro ao escrever no ficheiro %s\n", nome_arquivo);
        if(fclose(arq) == EOF){
            printf("Erro ao fechar o ficheiro %s\n", nome_arquivo);
            return;
        }
        return;   
    }

    for(int i = 0; i < n; i++){
        if(fprintf(arq, "%d\t", vetor[i]) < 0){
            printf("Erro ao escrever no ficheiro %s\n", nome_arquivo);
            if(fclose(arq) == EOF){
                printf("Erro ao fechar o ficheiro %s\n", nome_arquivo);
                return;
            }
            return;
        }
    }
    if(fclose(arq) == EOF){
        printf("Erro ao fechar o ficheiro %s\n", nome_arquivo);
    }
}

int main(){
    srand((unsigned int)time(NULL));
    int tam[] = {1000, 5000, 10000, 20000, 40000};
    int total_tams = 5;
    char nome_arquivo[100];

    for(int t = 0; t < total_tams; t++){
        int n = tam[t];
        int *vetor = (int*)malloc(n * sizeof(int));
        if(vetor == NULL){
            printf("Erro ao alocar memória para N = %d\n", n);
            return 1;
        }
        for(int rep = 1; rep <= 10; rep++){
            gerar_aleatorio(vetor, n);
            sprintf(nome_arquivo, "aleatorio_%d_%d.txt", n, rep);
            salvar_em_arquivo(nome_arquivo, vetor, n);
        }

        gerar_ordenado(vetor, n);
        sprintf(nome_arquivo, "ordenado_%d.txt", n);
        salvar_em_arquivo(nome_arquivo, vetor, n);
        
        gerar_invertido(vetor, n);
        sprintf(nome_arquivo, "invertido_%d.txt", n);
        salvar_em_arquivo(nome_arquivo, vetor, n);

        free(vetor);
    }

    return 0;
}