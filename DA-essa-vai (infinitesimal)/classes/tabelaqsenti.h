#ifndef TABELAQ_SENTI_H
#define TABELAQ_SENTI_H

#include <array>
#include <map>
#include <algorithm>
#include "constantes.h"

using StateSenti = std::array<int, QTD_AMEACAS>;

class QLearningTableSenti {
private:
    struct ArrayCompare {
        bool operator()(const StateSenti& a, const StateSenti& b) const {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
        }
    };

    std::map<StateSenti, std::map<int, float>, ArrayCompare> q_table;
    float learning_rate;
    float discount_factor;
    float exploration_rate;
    Random al;

public:
    QLearningTableSenti(float lr, float df, float er) 
        : learning_rate(lr), discount_factor(df), exploration_rate(er) {}

    int chooseAction(const StateSenti& state) {
        // Implementação existente...
    }

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
