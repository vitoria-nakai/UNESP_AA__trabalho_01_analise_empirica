import pandas as pd
import numpy as np

print("--> 1. Iniciando o processamento dos dados...")

# Carregar os dados
caminho_csv = 'metricas.csv'
df = pd.read_csv(caminho_csv, sep=';')

print(f"--> 2. Arquivo carregado com sucesso! Total de linhas brutas: {len(df)}")

if len(df) == 0:
    print("Aviso: O arquivo metricas.csv esta vazio! Rode o ./main.exe primeiro.")
else:
    # Agrupar por Algoritmo, Cenario e Tamanho (N) e calcular Media e Desvio-Padrao
    resumo = df.groupby(['Algoritmo', 'Cenario', 'N']).agg(
        Tempo_Medio_s=('Tempo_s', 'mean'),
        Tempo_DP_s=('Tempo_s', 'std'),
        Comp_Media=('Comparacoes', 'mean'),
        Comp_DP=('Comparacoes', 'std'),
        Trocas_Media=('Trocas', 'mean'),
        Trocas_DP=('Trocas', 'std')
    ).reset_index()

    resumo = resumo.fillna(0)

    # Salvar tabela consolidada
    saida_csv = 'resumo_estatistico.csv'
    resumo.to_csv(saida_csv, sep=';', index=False)

    print("--> 3. Dados consolidados salvos em:", saida_csv)
    print("\n--- Primeiras 10 linhas do Resumo Estatístico ---")
    print(resumo.head(10).to_string())

    input("\nPressione Enter para fechar...")