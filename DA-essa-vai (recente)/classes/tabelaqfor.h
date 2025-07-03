#ifndef QLEARNING_H
#define QLEARNING_H

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <random>
#include "constantes.h"

using namespace std;

constexpr size_t STATE_SIZE = QTD_INDIV + 1 + 3; // Extra + 3 inteiros fixos
using StateFor = array<int, STATE_SIZE>; // Substitui a tupla por array

class QLearningTableForrageador {
private:
    map<StateFor, map<int, float>> q_table;
    float learning_rate;
    float discount_factor;
    float exploration_rate;
    mt19937 gen{random_device{}()};

public:
    QLearningTableForrageador(float lr, float df, float er) : 
        learning_rate(lr), discount_factor(df), exploration_rate(er) {}

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