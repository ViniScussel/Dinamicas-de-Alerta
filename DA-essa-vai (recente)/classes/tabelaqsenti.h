#ifndef TABELAQSENTI_H
#define TABELAQSENTI_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <array>
#include "constantes.h"
#include "aleatorio.h"

using namespace std;

// Definir o tamanho do estado
constexpr int TAMANHO_ESTADO = QTD_AMEACAS;

// Define o estado como um array de inteiros
using StateSenti = array<int, TAMANHO_ESTADO>;

class QLearningTableSentinela {
private:
    // Comparador para array<int, N>
    struct ArrayCompare {
        bool operator()(const StateSenti& a, const StateSenti& b) const {
            return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }
    };

    map<StateSenti, map<int, float>, ArrayCompare> q_table;
    float learning_rate;
    float discount_factor;
    float exploration_rate;
    Random al;  // Gerador de aleatórios

public:
    QLearningTableSentinela(float lr, float df, float er)
        : learning_rate(lr), discount_factor(df), exploration_rate(er) {}

    // Escolher ação (1 a 4) com epsilon-greedy
    int chooseAction(const StateSenti& state) {
        if ((rand() / float(RAND_MAX)) < exploration_rate) {
            return al.randomicoEntre(1, 4);
        }

        auto it = q_table.find(state);
        if (it == q_table.end() || it->second.empty()) {
            return al.randomicoEntre(1, 4);
        }

        return max_element(it->second.begin(), it->second.end(),
            [](const pair<int, float>& a, const pair<int, float>& b) {
                return a.second < b.second;
            })->first;
    }

    // Atualizar Q-table
    void learn(const StateSenti& state, int action, float reward, const StateSenti& next_state) {
        auto& state_actions = q_table[state];
        if (state_actions.find(action) == state_actions.end()) {
            state_actions[action] = 0.0f;
        }

        float current_q = state_actions[action];
        float max_next_q = 0.0f;

        auto next_it = q_table.find(next_state);
        if (next_it != q_table.end() && !next_it->second.empty()) {
            max_next_q = max_element(next_it->second.begin(), next_it->second.end(),
                [](const pair<int, float>& a, const pair<int, float>& b) {
                    return a.second < b.second;
                })->second;
        }

        state_actions[action] = current_q + learning_rate *
                              (reward + discount_factor * max_next_q - current_q);
    }
};

#endif
