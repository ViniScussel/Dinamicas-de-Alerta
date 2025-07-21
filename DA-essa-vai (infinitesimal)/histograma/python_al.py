import pandas as pd
import matplotlib.pyplot as plt

# Carregar dados
df = pd.read_csv('/home/aurora/Documents/Dinamicas-de-Alerta/DA-essa-vai/tirar_dados/histograma.csv')

# Exemplo: agrupar em intervalos de 1000 (para facilitar a visualização)
df['Faixa'] = (df['Valor'] // 1000) * 1000
agrupado = df.groupby('Faixa')['Frequencia'].sum().reset_index()

plt.figure(figsize=(12, 6))
plt.bar(agrupado['Faixa'], agrupado['Frequencia'], width=900)
plt.xlabel('Faixa de valores')
plt.ylabel('Frequência')
plt.title('Histograma com 10.000.000 de amostras')
plt.grid(True)

# Salva como imagem
plt.savefig("histograma_plot.png")
print("Gráfico salvo como 'histograma_plot.png'")
