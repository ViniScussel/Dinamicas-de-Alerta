#ifndef AMBIENTE_H
#define AMBIENTE_H
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <map>
#include <tuple>
#include <string>

#include "../random_numbers/Random.h"
#include "json.hpp"
#include "QLearning.h"
#include "Individuos.h"
#include "Forrageadores.h"
#include "Sentinelas.h"
#include "Ameaca.h"
#include "Comida.h"
#include "constantes.h"


using namespace std;

class Environment {
    Random rdnum;
    Forager forager;
    std::vector<std::unique_ptr<Forager>> foragers;
    std::vector<std::unique_ptr<Sentinel>> sentinels;
    std::vector<std::unique_ptr<Threat>> threats;
    std::vector<std::unique_ptr<Food>> foods;
    
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

public:

    void giveID(){
        int mark = 1;
        for (int i = 0; i < foragers.size(); ++i){
            foragers[i]->id = mark;
            mark++;
        } for (int i = 1; i < sentinels.size()+1; ++i){
            sentinels[i-1]->id = mark;
            mark++;
        } for (int i = 1; i < threats.size()+1; ++i){
            threats[i-1]->id = mark;
            mark++;
        }
            
    }

    void populate(int num_foragers, int num_sentinels, int num_threats, int num_foods) {
        for (int i = 0; i < num_foragers; ++i)
            foragers.push_back(std::make_unique<Forager>());
        for (int i = 0; i < num_sentinels; ++i)
            sentinels.push_back(std::make_unique<Sentinel>());
        for (int i = 0; i < num_threats; ++i)
            threats.push_back(std::make_unique<Threat>());
        for (int i = 0; i < num_foods; ++i)
            foods.push_back(std::make_unique<Food>());
        
        giveID();
    }
    
    void detectThreats() {
        for (auto& sentinel : sentinels) {
            std::vector<float> detected(threats.size(), 0.0f);
            for (size_t i = 0; i < threats.size(); ++i) {
                if (threats[i]->isAlive()) {
                    detected[i] = threats[i]->getIntensity();
                }
            }
            sentinel->updateReadings(detected);
        }
    }

    vector<float> forageDetectThreats() {
        std::vector<float> detected(threats.size(), 0.0f);
        for (auto& sentinel : sentinels) {
            for (size_t i = 0; i < threats.size(); ++i) {
                if (threats[i]->isAlive()) {
                    detected[i] = threats[i]->getIntensity();
                }
            }
        }
        return detected;
    }
    
    void propagateAlerts() {
        for (auto& forager : foragers) {
            if(forager->isAlive()){
                std::vector<float> perceived(sentinels.size(), 0.0f);
                for (size_t s = 0; s < sentinels.size(); ++s) {
                    if(sentinels[s]->is_dominant && (rand() / float(RAND_MAX)) < DOMINANT_RANGE){
                        perceived[s] = *max_element(sentinels[s]->threat_readings.begin(),sentinels[s]->threat_readings.end());
                        sentinels[s]->last_action = *max_element(sentinels[s]->threat_readings.begin(),sentinels[s]->threat_readings.end());
                    }
                    else {perceived[s] = sentinels[s]->chooseAlertIntensity();}
                }
                forager->updateThreats(perceived);
            }
            
        }
    }

    void forager_alert(const std::vector<float> detected_threats){
        float max_threat_readings;
        float max_threat_dist;
        for (size_t i = 0; i < detected_threats.size()-1; ++i) {
            if(detected_threats[i]>detected_threats[i+1]){max_threat_readings=detected_threats[i];}
        }
        if(max_threat_readings<detected_threats[detected_threats.size()-1]){max_threat_readings=detected_threats[detected_threats.size()-1];}

        for(auto& forager: foragers){
            if(forager->isAlive()){
                int pointer_last = sentinels.size();
                forager->threat_levels[pointer_last] = max_threat_readings;
            }
            
        }
    }

    vector<float> positionOfNearestThreat(vector<float> forager){
        vector<float> positions;
        vector<vector<float>> vectors;
        if(threats.size()>1){
            for(auto& threat : threats){
                positions.push_back(distanceBetween(threat->getPosition(), forager));
                vectors.push_back(threat->getPosition());
            }
            int check = 1;
            while(check==1){
                for(int i=0; i<positions.size()-1; i++){
                    check = 0;
                    if(positions[i+1]<positions[i]){
                        float q = positions[i];
                        positions[i] = positions[i+1];
                        positions[i+1] = q;
                        vector<float> g = vectors[i];
                        vectors[i] = vectors[i+1];
                        vectors[i+1] = g;
                        check = 1;
                    }
                }
            }
            return vectors[0];
        } else if(threats.size()==1) {
            return threats[0]->getPosition();
        }
    }

    float function(float x){
        float value = -2 * log(0.0068*x)-1;
        return value;
    }

    void threatAttack(){
        for(auto& threat : threats){
            for(auto& forager : foragers){
                float distbet = distanceBetween(threat->getPosition(), forager->getPosition());
                if(distbet<=CERTAIN_ATTACK_DISTANCE){
                    forager->die();
                } else if(2*distbet/threat->intensity<=MIN_ATTACK_DISTANCE){
                    if(rdnum.getRand()<function(distbet)){forager->die();}
                }
            }
        }
    }
    
    float calculateRewardSentinel(int action, float threat_level, vector<float> forage_results) {
        float deadForagers;
        float r_food;
        for(int i = 0; i < forage_results.size(); i++){
            r_food+=forage_results[i]*0.8;
        }
        if (threat_level - action <= 0.2f) return 2.0f + r_food;
        if (threat_level - action <= 0.3f) return 1.5f + r_food;
        if (threat_level - action <= 0.5f) return 1.0f + r_food;
        if (threat_level - action <= 0.75f) return 0.5f + r_food;
        if (threat_level - action <= 1.0f) return -1.0f + r_food;

        for (auto& forager : foragers){
            deadForagers -= 15.0f;
        }

        return -2.5f + r_food + deadForagers;
    }
    
    float calculateRewardForager(int action, float threat_level, float forage_success) {
        float deadForagers;
        float reward = 0.0f;
        
        switch (action) {
            case 1: reward = (threat_level > 3.5f) ? 2.0f : -1.0f; break;
            case 2: reward = (threat_level > 2.5f && threat_level <= 3.5f) ? 1.0f : -0.5f; break;
            case 3: reward = (threat_level > 1.5f) ? 1.5f : -0.5f; break;
            case 4: reward = (threat_level < 1.5f) ? 0.5f : -1.0f; break;
            case 5: reward = (threat_level < 1.0f) ? forage_success * FOOD_REWARD : 
                   (threat_level < 2.0f) ? forage_success * FOOD_REWARD * 0.5f : -2.0f; break;
        }

        for (auto& forager : foragers){
            deadForagers -= 5;
        }
        
        return reward+deadForagers;
    }
    
    vector<float> forage_results(){
        std::vector<float> forage_vector;
        for (auto& forager : foragers) {
            forage_vector.push_back(forager->forage(foods));
        }
        return forage_vector;
    }
    
    void executeActions() {
        vector<float> forage_ = forage_results();
        
        for (size_t i = 0; i < foragers.size(); ++i) {
            auto& forager = foragers[i];
            if(forager->isAlive()){
                auto current_state = forager->getCurrentState();
                int action = forager->chooseAction();
                if(action==2){forager_alert(forageDetectThreats());}
                if(action==1){forager->escape(positionOfNearestThreat(forager->getPosition()));}
                
                float reward = calculateRewardForager(action, std::get<2>(current_state), forage_[i]);
                
                forager->executeAction(foods);
                
                auto new_state = forager->getCurrentState();
                forager->learn(current_state, action, reward, new_state);
            }
            
        }
        
        for (auto& sentinel : sentinels) {
            auto current_state = sentinel->getCurrentState();
            int action = sentinel->chooseAlertIntensity();
            float reward = calculateRewardSentinel(action, std::get<2>(current_state), forage_);
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
        threatAttack();
        moveThreats();
        detectThreats();
        executeActions();
        propagateAlerts();
        displayStatus();
        logSimulationStep(step);
        
        if (step % 5 == 0) {
            for (auto& food : foods) {
                if (!food->isAvailable() && (rand() % 4 == 0)) {
                    food = std::make_unique<Food>();
                }
            }
        }
    }

    vector<int> idOfDeadForagers(){
        vector<int> ids;
        ids.resize(0, 0);
        for(auto& forager : foragers){
            if(!forager->isAlive()){
                ids.push_back(forager->id);
            }
        }
        return ids;
    }
    

    json generateSimulationJson(int step) {

        json simulationJson;

        // Metadados da simulação
        simulationJson["step"] = step;
        simulationJson["counts"] = {
        {"dead_foragers", idOfDeadForagers().size()},
        {"foragers", foragers.size()-idOfDeadForagers().size()},
        {"sentinels", sentinels.size()},
        {"threats", threats.size()},
        {"food_sources", foods.size()}
        };

        // Detalhes dos agentes
        json agentsJson = json::array();

        // Adiciona ameaças
        for (const auto& threat : threats) {
        if (threat->isAlive()) {
        agentsJson.push_back(threat->toJson());
        }
        }

        // Adiciona fontes de comida
        for (const auto& food : foods) {
        if (dynamic_cast<Food*>(food.get())->isAvailable()) {
        agentsJson.push_back(food->toJson());
        }
        }

        // Adiciona sentinelas
        for (const auto& sentinel : sentinels) {
        agentsJson.push_back(sentinel->toJson());
        }

        // Adiciona forrageadores
        for (const auto& forager : foragers) {
        if(forager->isAlive()){
            agentsJson.push_back(forager->toJson());}
        }
        
        json{
            {"number_of_dead_foragers: ", idOfDeadForagers().size()}
        };

        simulationJson["agents"] = agentsJson;

        return simulationJson;
    }

    void logSimulationStep(int step) {

    json stepJson = generateSimulationJson(step);

    // Escreve no arquivo (append)
    ofstream logFile("simulation_log.json", std::ios::app);
    if (step == 1) {
    logFile << "[\n"; // Inicia o array JSON
    } 
    else {
    logFile << ",\n"; // Separador entre steps
    }
    logFile << stepJson.dump(4);

    // No final da simulação, você precisa fechar o array
    // simulationEnd(); // Chamar esta função no final
    }

    void simulationEnd() {
    std::ofstream logFile("simulation_log.json", std::ios::app);
    logFile << "\n]"; // Fecha o array JSON
    }

    void runSimulation() {
        for (int i = 0; i < SIMULATION_STEPS; ++i) {
            simulationStep(i + 1);
        }
        simulationEnd();
    }
};

#endif