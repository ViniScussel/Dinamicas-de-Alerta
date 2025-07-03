#include <iostream>
#include <fstream>
#include <random>
#include <unordered_map>

using namespace std;

class Random {
    mt19937 gen;
    uniform_int_distribution<int> dist;

public:
    Random(int min, int max) : gen(random_device{}()), dist(min, max) {}

    int randomicoEntre() {
        return dist(gen);
    }
};

int main() {
    const int num_amostras = 10000000;
    const int minimo = 1;
    const int maximo = 100000;

    Random rnd(minimo, maximo);
    unordered_map<int, int> histograma;

    // Coleta os dados
    for (int i = 0; i < num_amostras; ++i) {
        int valor = rnd.randomicoEntre();
        histograma[valor]++;
    }

    // Exporta para CSV
    ofstream arquivo_csv("histograma.csv");
    if (!arquivo_csv.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV para escrita." << endl;
        return 1;
    }

    arquivo_csv << "Valor,Frequencia\n";
    for (int i = minimo; i <= maximo; ++i) {
        int freq = histograma.count(i) ? histograma[i] : 0;
        arquivo_csv << i << "," << freq << "\n";
    }

    arquivo_csv.close();
    cout << "Arquivo 'histograma.csv' com " << num_amostras << " amostras gerado com sucesso!" << endl;

    return 0;
}
