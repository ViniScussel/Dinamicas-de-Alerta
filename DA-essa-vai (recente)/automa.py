import re
import subprocess
import os

# Arquivos
HEADER_FILE = "classes/constantes.h"
CPP_FILE = "main.cpp"
EXECUTABLE = "./simulacao"

# Ranges dos parâmetros (início, fim, incremento)
PARAMETROS = {
    "TAMANHO_MAPA": {"inicio": 10000, "fim": 100000, "incr": 500},  # 10k → 20k (+500)
    "QTD_INDIV": {"inicio": 10, "fim": 500, "incr": 10},           # 10 → 500 (+10)
    "PORCENT_SENTI": {"inicio": 5, "fim": 30, "incr": 5},         # 5% → 30% (+5)
    "QTD_AMEACAS": {"inicio": 1, "fim": 5, "incr": 1}             # 1 → 5 (+1)
}

def modificar_constantes(params):
    """Modifica os parâmetros no arquivo .h"""
    with open(HEADER_FILE, 'r') as file:
        conteudo = file.read()
    
    # Substitui cada parâmetro
    for param, valor in params.items():
        # Padrão para: "const int PARAM = VALOR;"
        padrao = rf'(const int {param}\s*=\s*)(\d+)(;)'
        conteudo = re.sub(padrao, rf'\g<1>{valor}\g<3>', conteudo)
    
    # Salva as alterações
    with open(HEADER_FILE, 'w') as file:
        file.write(conteudo)
    
    print(f"⚙ Parâmetros atualizados: {params}")

def compilar_programa():
    """Compila o programa C++ (SEMPRE, mesmo se já existir o executável)"""
    print("🔨 Compilando...")
    subprocess.run(["g++", CPP_FILE, "-o", "simulacao"], check=True)
    print("✅ Compilação concluída!")

def executar_simulacao():
    """Roda a simulação"""
    print("🚀 Executando...")
    subprocess.run([EXECUTABLE], check=True)

def main():
    # Verifica se os arquivos existem
    if not os.path.exists(HEADER_FILE):
        raise FileNotFoundError(f"Arquivo {HEADER_FILE} não encontrado!")
    if not os.path.exists(CPP_FILE):
        raise FileNotFoundError(f"Arquivo {CPP_FILE} não encontrado!")
    
    # Loop aninhado para cada parâmetro
    for tam_mapa in range(
        PARAMETROS["TAMANHO_MAPA"]["inicio"],
        PARAMETROS["TAMANHO_MAPA"]["fim"] + 1,
        PARAMETROS["TAMANHO_MAPA"]["incr"]
    ):
        for qtd_indiv in range(
            PARAMETROS["QTD_INDIV"]["inicio"],
            PARAMETROS["QTD_INDIV"]["fim"] + 1,
            PARAMETROS["QTD_INDIV"]["incr"]
        ):
            for porcent_senti in range(
                PARAMETROS["PORCENT_SENTI"]["inicio"],
                PARAMETROS["PORCENT_SENTI"]["fim"] + 1,
                PARAMETROS["PORCENT_SENTI"]["incr"]
            ):
                for qtd_ameaca in range(
                    PARAMETROS["QTD_AMEACAS"]["inicio"],
                    PARAMETROS["QTD_AMEACAS"]["fim"] + 1,
                    PARAMETROS["QTD_AMEACAS"]["incr"]
                ):
                    print(f"\n🔵 Configuração: TAM_MAPA={tam_mapa}, INDIV={qtd_indiv}, SENTI={porcent_senti}%, AMEACA={qtd_ameaca}")
                    
                    # 1. Modifica o arquivo .h
                    modificar_constantes({
                        "TAMANHO_MAPA": tam_mapa,
                        "QTD_INDIV": qtd_indiv,
                        "PORCENT_SENTI": porcent_senti,
                        "QTD_AMEACAS": qtd_ameaca
                    })
                    
                    # 2. Compila (SEMPRE, independente de existir executável)
                    compilar_programa()
                    
                    # 3. Executa
                    executar_simulacao()
    
    print("\n✅ Todas as combinações foram simuladas!")

if __name__ == "__main__":
    main()