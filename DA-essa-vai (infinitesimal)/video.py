import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Configurações do ambiente
TAMANHO_MAPA = 10000
ARQ_INDVS = "movimentos.csv"
ARQ_AMEACAS = "ameacas.csv"

# Lê os dados
df = pd.read_csv(ARQ_INDVS, names=[
    "Execucao", "ID", "Tipo", "Vivo", "Acao", "Idade", "Comida", "X", "Y"
])
df_ameacas = pd.read_csv(ARQ_AMEACAS, names=["Execucao", "ID", "Intensidade", "X", "Y"])

# Frames
frames = sorted(df["Execucao"].unique())

# Paleta
cores_indv = {0: "blue", 1: "green"}  # Forrageador, Sentinela
cor_ameaca = "red"

# Inicializa figura
fig, ax = plt.subplots(figsize=(7, 7))
scatter_indv = ax.scatter([], [], s=[], c=[], label="Indivíduos")
scatter_ameacas = ax.scatter([], [], s=[], c=[], marker='X', label="Ameaças")

def init():
    ax.set_xlim(0, TAMANHO_MAPA)
    ax.set_ylim(0, TAMANHO_MAPA)
    ax.set_title("Simulação com Ameaças")
    ax.legend(loc="upper right")
    return scatter_indv, scatter_ameacas

def update(frame):
    dados = df[df["Execucao"] == frame]
    dados_am = df_ameacas[df_ameacas["Execucao"] == frame]

    # Indivíduos
    x = dados["X"]
    y = dados["Y"]
    tamanhos = [50 if v == 1 else 10 for v in dados["Vivo"]]
    cores_ponto = [cores_indv[t] if v == 1 else "gray" for t, v in zip(dados["Tipo"], dados["Vivo"])]
    scatter_indv.set_offsets(list(zip(x, y)))
    scatter_indv.set_sizes(tamanhos)
    scatter_indv.set_color(cores_ponto)
    scatter_indv.set_facecolor(cores_ponto)
    scatter_indv.set_linewidth(3)

    # Ameaças
    xa = dados_am["X"]
    ya = dados_am["Y"]
    intens = dados_am["Intensidade"] * 30  # escala de tamanho
    scatter_ameacas.set_offsets(list(zip(xa, ya)))
    scatter_ameacas.set_sizes(intens)
    scatter_ameacas.set_color(cor_ameaca)

    ax.set_title(f"Execução: {frame}")
    return scatter_indv, scatter_ameacas

ani = animation.FuncAnimation(
    fig, update, frames=frames, init_func=init,
    blit=True, interval=10 #ms p frame
)

# Exibir ou salvar
ani.save("simulacao_com_ameacas.mp4", fps=100, dpi=150)
plt.tight_layout()
plt.show()
