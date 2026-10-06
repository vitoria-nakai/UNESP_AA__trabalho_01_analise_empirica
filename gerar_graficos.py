import pandas as pd
import matplotlib.pyplot as plt
import os

# 1. Carregar os dados consolidados
df = pd.read_csv('resumo_estatistico.csv', sep=';')

# Criar pasta para guardar as imagens
os.makedirs('graficos', exist_ok=True)

cenarios = df['Cenario'].unique()
algoritmos = df['Algoritmo'].unique()

cores = {
    'Selection Sort': '#e41a1c',
    'Insertion Sort': '#377eb8',
    'Merge Sort': '#4daf4a',
    'Quick Sort': '#984ea3',
    'Heap Sort': '#ff7f00'
}

# ====================================================================
# 1. GRÁFICOS DE TEMPO (Escala Linear)
# ====================================================================
for cenario in cenarios:
    plt.figure(figsize=(9, 6))
    df_cenario = df[df['Cenario'] == cenario]

    for alg in algoritmos:
        dados_alg = df_cenario[df_cenario['Algoritmo'] == alg].sort_values('N')
        if not dados_alg.empty:
            plt.errorbar(
                dados_alg['N'],
                dados_alg['Tempo_Medio_s'],
                yerr=dados_alg['Tempo_DP_s'],
                label=alg,
                color=cores.get(alg, None),
                marker='o',
                capsize=4,
                linewidth=1.8
            )

    plt.title(f'Tempo de Execução Médio - Cenário {cenario}', fontsize=13, fontweight='bold')
    plt.xlabel('Tamanho da Entrada (N)', fontsize=11)
    plt.ylabel('Tempo de Execução Médio (s)', fontsize=11)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend()
    plt.tight_layout()

    plt.savefig(f'graficos/tempo_{cenario.lower()}.png', dpi=300)
    plt.close()

# ====================================================================
# 2. GRÁFICOS DE COMPARAÇÕES (Versão Logarítmica - Todas as curvas visíveis)
# ====================================================================
for cenario in cenarios:
    plt.figure(figsize=(9, 6))
    df_cenario = df[df['Cenario'] == cenario]

    for alg in algoritmos:
        dados_alg = df_cenario[df_cenario['Algoritmo'] == alg].sort_values('N')
        if not dados_alg.empty:
            plt.plot(
                dados_alg['N'],
                dados_alg['Comp_Media'],
                label=alg,
                color=cores.get(alg, None),
                marker='s',
                linewidth=1.8
            )

    plt.title(f'Comparações de Chaves (Escala Logarítmica) - Cenário {cenario}', fontsize=13, fontweight='bold')
    plt.xlabel('Tamanho da Entrada (N)', fontsize=11)

    # --- AQUI FICA A LINHA CHAVE ---
    plt.yscale('log')
    plt.ylabel('Média de Comparações (C_comp) [Escala Log]', fontsize=11)
    # -------------------------------

    plt.grid(True, which='both', linestyle='--', alpha=0.6)
    plt.legend()
    plt.tight_layout()

    nome_ficheiro = f'graficos/comparacoes_{cenario.lower()}_log.png'
    plt.savefig(nome_ficheiro, dpi=300)
    plt.close()
    print(f'-> Gráfico gerado com escala logarítmica: {nome_ficheiro}')

print('\nTodos os gráficos foram atualizados!')