#ifndef FORRAGEADORES_H
#define FORRAGEADORES_H

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

#include "Individuos.h"
#include "QLearning.h"
#include "constantes.h"
#include "Comida.h"

using namespace std;

class Forager : public Individual {
    std::vector<float> threat_levels;
    QLearningTable q_table;
    int last_action;
    float food_collected;
    
public:
    Forager() : q_table(LEARNING_RATE, DISCOUNT_FACTOR, EXPLORATION_RATE), food_collected(0.0f) {
        threat_levels.resize(4, 0.0f);
        last_action = 0;
    }
    
    void updateThreats(const std::vector<float>& perceived_threats) {
        for (size_t i = 0; i < perceived_threats.size(); ++i) {
            threat_levels[i] = perceived_threats[i];
        }
    }
    
    float responseTime() const {
        float max_threat = *max_element(threat_levels.begin(), threat_levels.end());
        return 1.0f / (1.0f + max_threat);
    }
    
    int chooseAction() {
        float max_threat = *max_element(threat_levels.begin(), threat_levels.end());
        auto state = std::make_tuple(x, y, max_threat);
        last_action = q_table.chooseAction(state);
        return last_action;
    }
    
    float forage(vector<unique_ptr<Food>>& foods) {
        if (last_action != 5) return 0.0f;
        
        float total_collected = 0.0f;
        vector<float> pos = getPosition();
        
        for (auto& food : foods) {
            if (food->isAvailable()) {
                float distance = distanceTo(food->getPosition());
                if (distance <= FORAGE_RADIUS) {
                    float amount = 0.1f * (1.0f - distance/FORAGE_RADIUS);
                    total_collected += food->collect(amount);
                }
            }
        }
        
        food_collected += total_collected;
        return total_collected;
    }
    
    void executeAction(std::vector<std::unique_ptr<Food>>& foods) {
        switch (last_action) {
            case 1: // Fugir
                moveRandomly();
                break;
            case 2: // Alertar
                break;
            case 3: // Buscar abrigo
                moveRandomly();
                break;
            case 4: // Vigilância
                if (rand() % 3 == 0) moveRandomly();
                break;
            case 5: // Forragear
                if (!foods.empty()) {
                    std::vector<float> closest_food_pos;
                    float min_dist = 2.0f;
                    
                    for (auto& food : foods) {
                        if (food->isAvailable()) {
                            float dist = distanceTo(food->getPosition());
                            if (dist < min_dist) {
                                min_dist = dist;
                                closest_food_pos = food->getPosition();
                            }
                        }
                    }
                    
                    if (!closest_food_pos.empty() && min_dist > FORAGE_RADIUS) {
                        moveTowards(closest_food_pos);
                    }
                }
                break;
        }
    }
    
    void learn(const std::tuple<float, float, float>& state, int action, float reward,
               const std::tuple<float, float, float>& new_state) {
        q_table.learn(state, action, reward, new_state);
    }
    
    std::tuple<float, float, float> getCurrentState() const {
        float max_threat = *max_element(threat_levels.begin(), threat_levels.end());
        return std::make_tuple(x, y, max_threat);
    }
    
    float getFoodCollected() const {
        return food_collected;
    }
    
    void resetFoodCollected() {
        food_collected = 0.0f;
    }
    
    void displayStatus() const {
        const char* action_names[] = {"None", "Flee", "Alert", "Shelter", "Vigilance", "Forage"};
        
        std::cout << "Forager (" << x << ", " << y << ") - "
                 << "Threats: [";
        for (float threat : threat_levels) {
            std::cout << threat << " ";
        }
        std::cout << "] Action: " << action_names[last_action] 
                 << " Food: " << food_collected << std::endl;
    }
};

#endif