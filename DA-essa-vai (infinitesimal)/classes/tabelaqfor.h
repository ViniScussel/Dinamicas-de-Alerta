#ifndef QLEARNING_H
#define QLEARNING_H

#include <iostream>
#include <array>
#include <map>
#include <algorithm>
#include <random>
#include "constantes.h"

using namespace std;

// Estado compactado em 9 quadrantes (3x3) + 3 informações extras
constexpr int STATE_SIZE = QTD_AMEACAS + 3;
using StateFor = std::array<int, STATE_SIZE>;

class QLearningTableForrageador {
private:
    // Comparador para o map com arrays
    struct ArrayCompare {
        bool operator()(const StateFor& a, const StateFor& b) const {
            return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }
    };

    float calcularDistanciaToroidal(int x1, int y1, int x2, int y2) const {
        // Calcula diferenças absolutas
        int dx = abs(x1 - x2);
        int dy = abs(y1 - y2);
        
        // Considera wrap-around (menor distância entre os dois caminhos possíveis)
        dx = min(dx, TAMANHO_MAPA - dx);
        dy = min(dy, TAMANHO_MAPA - dy);
        
        // Distância euclidiana
        return sqrt(dx*dx + dy*dy);
    }

    map<StateFor, map<int, float>, ArrayCompare> q_table;
    float learning_rate;
    float discount_factor;
    float exploration_rate;
    mt19937 gen{random_device{}()};

    // Calcula o quadrante (3x3) para uma posição
    int calcularQuadrante(int x, int y) const {
        const int quadrant_size_x = TAMANHO_MAPA / 3;
        const int quadrant_size_y = TAMANHO_MAPA / 3;
        return (y / quadrant_size_y) * 3 + (x / quadrant_size_x);
    }

public:
    QLearningTableForrageador(float lr, float df, float er) : 
        learning_rate(lr), discount_factor(df), exploration_rate(er) {}

    // Método auxiliar para criar estado compactado
    StateFor criarEstado(const vector<pair<int,int>>& posicoesIndividuos,
                        const vector<int>& ameacasOuvidas,
                        const vector<int>& posAmeacaProxima,
                        int comida) const {
        StateFor estado = {};
        
        // 1. Contagem por quadrante
        for (const auto& pos : posicoesIndividuos) {
            int quad = calcularQuadrante(pos.first, pos.second);
            estado[quad]++;
        }
        
        // 2. Intensidade máxima de ameaça ouvida
        if (!ameacasOuvidas.empty()) {
            estado[9] = *max_element(ameacasOuvidas.begin(), ameacasOuvidas.end());
        }
        
        // 3. Distância à ameaça mais próxima (com wrap-around)
        if (!posAmeacaProxima.empty() && !posicoesIndividuos.empty()) {
            // Pega posição do próprio indivíduo (primeira posição)
            auto& pos = posicoesIndividuos[0]; 
            
            // Calcula distância com wrap-around
            float dist = calcularDistanciaToroidal(
                pos.first, pos.second,
                posAmeacaProxima[0], posAmeacaProxima[1]
            );
            
            // Normalização (0-100)
            float dist_max = TAMANHO_MAPA * sqrt(2); // Diagonal máxima
            float normalized = min(max(dist / dist_max, 0.0f), 1.0f);
            estado[10] = static_cast<int>(normalized * 100);
        }
        
        // 4. Nível de comida
        estado[11] = comida;
        
        return estado;
    }

    int chooseAction(const StateFor& state) {
        uniform_real_distribution<float> dist(0.0, 1.0);
        if (dist(gen) < exploration_rate) {
            uniform_int_distribution<> action_dist(1, 5);
            return action_dist(gen);
        }
        
        const auto& actions = q_table[state];
        if (actions.empty()) {
            uniform_int_distribution<> action_dist(1, 5);
            return action_dist(gen);
        }
        
        return max_element(actions.begin(), actions.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    }

    void learn(const StateFor& state, int action, float reward, const StateFor& next_state) {
        float& current_q = q_table[state][action];
        float max_next_q = 0.0f;
        
        if (!q_table[next_state].empty()) {
            max_next_q = max_element(q_table[next_state].begin(), q_table[next_state].end(),
                [](const auto& a, const auto& b) { return a.second < b.second; })->second;
        }
        
        current_q += learning_rate * (reward + discount_factor * max_next_q - current_q);
    }
};
#endif