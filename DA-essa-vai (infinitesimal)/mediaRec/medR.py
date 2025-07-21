import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

sns.set(style="whitegrid")

# Carrega o CSV
df = pd.read_csv("/home/aurora/Documentos/Dinamicas-de-Alerta/DA-essa-vai/recompensas_rodada.csv", names=["Execucao", "Media_Recompensa"])

df["Execucao"] = pd.to_numeric(df["Execucao"], errors="coerce")
df["Media_Recompensa"] = pd.to_numeric(df["Media_Recompensa"], errors="coerce")
df = df.dropna()

# Remove dados inválidos (evita log de 0 ou negativo)
df = df[df["Media_Recompensa"] > 0]

# Dados
x = df["Execucao"].values
y = df["Media_Recompensa"].values

# Ajuste exponencial: y = a * exp(b * x)
# ln(y) = ln(a) + b * x  → regressão linear
log_y = np.log(y)
b, log_a = np.polyfit(x, log_y, 1)
a = np.exp(log_a)
y_fit = a * np.exp(b * x)

# Gráfico
plt.figure(figsize=(14, 7))

# Curva original
plt.plot(x, y, color='gray', alpha=0.5, label="Média das Recompensas")

# Curva ajustada
plt.plot(x, y_fit, color='red', linewidth=2, linestyle='--',
         label=f"Ajuste Exponencial: $y = {a:.2f}e^{{{b:.2e}x}}$")

# Rótulos e estilo
plt.xlabel("Execução", fontsize=12)
plt.ylabel("Média da Recompensa", fontsize=12)
plt.title("Ajuste Exponencial Global das Recompensas", fontsize=14)
plt.legend()
plt.grid(True, linestyle=':', alpha=0.4)
plt.tight_layout()
plt.savefig("grafico_ajuste_exponencial_global.png", dpi=300)
plt.show()

#=============================================INDV===============================================

# Carrega o CSV
df = pd.read_csv("/home/aurora/Documentos/Dinamicas-de-Alerta/DA-essa-vai/qtd_indv.csv", names=["Execucao", "qtd_Indv"])

df["Execucao"] = pd.to_numeric(df["Execucao"], errors="coerce")
df["qtd_Indv"] = pd.to_numeric(df["qtd_Indv"], errors="coerce")
df = df.dropna()

# Remove dados inválidos (evita log de 0 ou negativo)
df = df[df["qtd_Indv"] > 0]

# Dados
x = df["Execucao"].values
y = df["qtd_Indv"].values

# Ajuste exponencial: y = a * exp(b * x)
# ln(y) = ln(a) + b * x  → regressão linear
log_y = np.log(y)
b, log_a = np.polyfit(x, log_y, 1)
a = np.exp(log_a)
y_fit = a * np.exp(b * x)

# Gráfico
plt.figure(figsize=(14, 7))

# Curva original
plt.plot(x, y, color='gray', alpha=0.5, label="Média das Recompensas")

# Curva ajustada
plt.plot(x, y_fit, color='red', linewidth=2, linestyle='--',
         label=f"Ajuste Exponencial: $y = {a:.2f}e^{{{b:.2e}x}}$")

# Rótulos e estilo
plt.xlabel("Execução", fontsize=12)
plt.ylabel("Quantidade de Indivíduos", fontsize=12)
plt.title("Quantidade de Indv. p/ Execução", fontsize=14)
plt.legend()
plt.grid(True, linestyle=':', alpha=0.4)
plt.tight_layout()
plt.savefig("qtdindv.png", dpi=300)
plt.show()