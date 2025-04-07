#ifndef AMBIENTE_H
#define AMBIENTE_H
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

#include "QLearning.h"
#include "Individuos.h"
#include "Forrageadores.h"
#include "Sentinelas.h"
#include "Ameaca.h"
#include "Comida.h"
#include "constantes.h"


using namespace std;


class Environment {
    std::vector<std::unique_ptr<Forager>> foragers;
    std::vector<std::unique_ptr<Sentinel>> sentinels;
    std::vector<std::unique_ptr<Threat>> threats;
    std::vector<std::unique_ptr<Food>> foods;
    
    /* 
    float environmentalNoise(const std::vector<float>& pos1, const std::vector<float>& pos2) const {
        float distance = distanceBetween(pos1, pos2);
        if (distance < 0.001f) return 1.0f;
        
        const float attenuation = 0.0015f;
        float decay = exp(-attenuation * distance);
        float pollution = 0.8f + (rand() / float(RAND_MAX)) * 0.4f;
        float humidity = 0.7f + (rand() / float(RAND_MAX)) * 0.6f;
        return decay * pollution * humidity;
    }

    float distanceBetween(const std::vector<float>& pos1, const std::vector<float>& pos2) const {
        float dx = pos1[0] - pos2[0];
        float dy = pos1[1] - pos2[1];
        return sqrt(dx*dx + dy*dy);
    }
    */

public:
    void populate(int num_foragers, int num_sentinels, int num_threats, int num_foods) {
        for (int i = 0; i < num_foragers; ++i)
            foragers.push_back(std::make_unique<Forager>());
        for (int i = 0; i < num_sentinels; ++i)
            sentinels.push_back(std::make_unique<Sentinel>());
        for (int i = 0; i < num_threats; ++i)
            threats.push_back(std::make_unique<Threat>());
        for (int i = 0; i < num_foods; ++i)
            foods.push_back(std::make_unique<Food>());
    }
    
    void detectThreats() {
        for (auto& sentinel : sentinels) {
            std::vector<float> detected(threats.size(), 0.0f);
            for (size_t i = 0; i < threats.size(); ++i) {
                if (threats[i]->isAlive()) {
                    float distance = sentinel->distanceTo(threats[i]->getPosition());
                    detected[i] = threats[i]->getIntensity();
                }
            }
            sentinel->updateReadings(detected);
        }
    }
    
    void propagateAlerts() {
        for (auto& forager : foragers) {
            std::vector<float> perceived(sentinels.size(), 0.0f);
            for (size_t s = 0; s < sentinels.size(); ++s) {
                perceived[s] = sentinels[s]->chooseAlertIntensity();
            }
            forager->updateThreats(perceived);
        }
    }
    
    float calculateRewardSentinel(int action, float threat_level) {
        if (threat_level - action <= 0.2f) return 2.0f;
        if (threat_level - action <= 0.3f) return 1.5f;
        if (threat_level - action <= 0.5f) return 1.0f;
        if (threat_level - action <= 0.75f) return 0.5f;
        if (threat_level - action <= 1.0f) return -1.0f;
        return -2.5f;
    }
    
    float calculateRewardForager(int action, float threat_level, float forage_success) {
        float reward = 0.0f;
        
        switch (action) {
            case 1: reward = (threat_level > 3.5f) ? 2.0f : -1.0f; break;
            case 2: reward = (threat_level > 2.5f && threat_level <= 3.5f) ? 1.0f : -0.5f; break;
            case 3: reward = (threat_level > 1.5f) ? 1.5f : -0.5f; break;
            case 4: reward = (threat_level < 1.5f) ? 0.5f : -1.0f; break;
            case 5: reward = (threat_level < 1.0f) ? forage_success * FOOD_REWARD : 
                   (threat_level < 2.0f) ? forage_success * FOOD_REWARD * 0.5f : -2.0f; break;
        }
        
        return reward;
    }
    
    void executeActions() {
        std::vector<float> forage_results;
        for (auto& forager : foragers) {
            forage_results.push_back(forager->forage(foods));
        }
        
        for (size_t i = 0; i < foragers.size(); ++i) {
            auto& forager = foragers[i];
            auto current_state = forager->getCurrentState();
            int action = forager->chooseAction();
            
            float reward = calculateRewardForager(action, std::get<2>(current_state), forage_results[i]);
            
            forager->executeAction(foods);
            
            auto new_state = forager->getCurrentState();
            forager->learn(current_state, action, reward, new_state);
        }
        
        for (auto& sentinel : sentinels) {
            auto current_state = sentinel->getCurrentState();
            int action = sentinel->chooseAlertIntensity();
            float reward = calculateRewardSentinel(action, std::get<2>(current_state));
            auto new_state = sentinel->getCurrentState();
            sentinel->learn(current_state, action, reward, new_state);
        }
    }
    
    void moveThreats() {
        for (auto& threat : threats) {
            threat->move();
        }
    }
    
    void displayStatus() const {
        std::cout << "\n=== SIMULATION STATUS ===" << std::endl;
        std::cout << "Foragers: " << foragers.size() << std::endl;
        std::cout << "Sentinels: " << sentinels.size() << std::endl;
        std::cout << "Threats: " << threats.size() << std::endl;
        std::cout << "Food sources: " << foods.size() << std::endl;
        
        std::cout << "\nDetails:" << std::endl;
        for (const auto& threat : threats)
            if (threat->isAlive()) threat->displayStatus();
        for (const auto& food : foods)
            if (food->isAvailable()) food->displayStatus();
        for (const auto& sentinel : sentinels)
            sentinel->displayStatus();
        for (const auto& forager : foragers)
            forager->displayStatus();
    }
    
    void simulationStep(int step) {
        std::cout << "\n=== STEP " << step << " ===" << std::endl;
        moveThreats();
        detectThreats();
        executeActions();
        propagateAlerts();
        displayStatus();
        
        if (step % 5 == 0) {
            for (auto& food : foods) {
                if (!food->isAvailable() && (rand() % 4 == 0)) {
                    food = std::make_unique<Food>();
                }
            }
        }
    }
    
    void runSimulation() {
        for (int i = 0; i < SIMULATION_STEPS; ++i) {
            simulationStep(i + 1);
        }
    }
};

#endif