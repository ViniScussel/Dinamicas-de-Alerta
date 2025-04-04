#ifndef QLEARNING_H
#define QLEARNING_H

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <map>
#include <tuple>

using namespace std;

using State = tuple<float, float, float>;

class QLearningTable {
private:
    map<State, map<int, float>> q_table;
    float learning_rate;
    float discount_factor;
    float exploration_rate;

public:
    QLearningTable(float lr, float df, float er) 
        : learning_rate(lr), discount_factor(df), exploration_rate(er) {}

    // Escolhe uma ação baseada no estado atual (ε-greedy)
    int chooseAction(const State& state) {
        if ((rand() / float(RAND_MAX)) < exploration_rate) {
            // Exploração: ação aleatória
            return 1 + rand() % 4; // Ações de 1 a 4
        }
        
        // Exploitation: ação com maior valor Q
        auto& actions = q_table[state];
        if (actions.empty()) {
            return 1 + rand() % 4; // Ação aleatória se estado não conhecido
        }
        
        return max_element(actions.begin(), actions.end(),
            [](const pair<int, float>& a, const pair<int, float>& b) {
                return a.second < b.second;
            })->first;
    }

    // Atualiza a Q-table com a nova experiência
    void learn(const State& state, int action, float reward, const State& next_state) {
        float current_q = q_table[state][action];
        float max_next_q = 0.0f;
        
        if (!q_table[next_state].empty()) {
            max_next_q = max_element(q_table[next_state].begin(), q_table[next_state].end(),
                [](const pair<int, float>& a, const pair<int, float>& b) {
                    return a.second < b.second;
                })->second;
        }
        
        // Equação de Q-learning
        q_table[state][action] = current_q + learning_rate * 
                                (reward + discount_factor * max_next_q - current_q);
    }

    // Retorna o valor Q para um estado-ação específico (para debug)
    float getQValue(const State& state, int action) const {
        auto state_it = q_table.find(state);
        if (state_it == q_table.end()) return 0.0f;
        
        auto action_it = state_it->second.find(action);
        if (action_it == state_it->second.end()) return 0.0f;
        
        return action_it->second;
    }
};
#endif