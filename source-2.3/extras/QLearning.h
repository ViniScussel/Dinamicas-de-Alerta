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
#include <string>

#include "json.hpp"
#include "constantes.h"

using namespace std;


using State = tuple<float, float, float>;

class QLearningTable {
private:
    std::map<std::tuple<float, float, float>, std::map<int, float>> q_table;
    float learning_rate;
    float discount_factor;
    float exploration_rate;

public:
    QLearningTable(float lr, float df, float er) 
        : learning_rate(lr), discount_factor(df), exploration_rate(er) {}

    int chooseAction(const std::tuple<float, float, float>& state) {
        if ((rand() / float(RAND_MAX)) < exploration_rate) {
            return 1 + rand() % 5;
        }
        
        auto& actions = q_table[state];
        if (actions.empty()) {
            return 1 + rand() % 5;
        }
        
        return max_element(actions.begin(), actions.end(),
            [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second < b.second;
            })->first;
    }

    int chooseActionSentinela(const std::tuple<float, float, float>& state) {
        if ((rand() / float(RAND_MAX)) < exploration_rate) {
            return 1 + rand() % 4;
        }
        
        auto& actions = q_table[state];
        if (actions.empty()) {
            return 1 + rand() % 4;
        }
        
        return max_element(actions.begin(), actions.end(),
            [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second < b.second;
            })->first;
    }

    void learn(const std::tuple<float, float, float>& state, int action, float reward, 
                const std::tuple<float, float, float>& next_state) {
        float current_q = q_table[state][action];
        float max_next_q = 0.0f;
        
        if (!q_table[next_state].empty()) {
            max_next_q = max_element(q_table[next_state].begin(), q_table[next_state].end(),
                [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                    return a.second < b.second;
                })->second;
        }
        
        q_table[state][action] = current_q + learning_rate * 
                                (reward + discount_factor * max_next_q - current_q);
    }
};
#endif