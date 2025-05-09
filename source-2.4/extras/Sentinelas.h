#ifndef SENTINELAS_H
#define SENTINELAS_H

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
#include "Individuos.h"
#include "constantes.h"
#include "QLearning.h"
#include "Comida.h"

using json = nlohmann::json;
using namespace std;


class Sentinel : public Individual {
    
    QLearningTable q_table;
    
    
public:
//Isso tbm n deveria estar aqui, mas só funciona assim
    int last_action;
    bool is_dominant;
    std::vector<float> threat_readings;

    Sentinel() : is_dominant(rand() % 2), q_table(LEARNING_RATE, DISCOUNT_FACTOR, EXPLORATION_RATE) {
        threat_readings.resize(THREAT_NUMBER, 0.0f);
        last_action = 0;
    }
    
    void updateReadings(const std::vector<float>& detected_threats) {
        for (size_t i = 0; i < detected_threats.size(); ++i) {
            threat_readings[i] = detected_threats[i];
        }
    }
    
    int chooseAlertIntensity() {
        float max_threat = *max_element(threat_readings.begin(), threat_readings.end());
        auto state = std::make_tuple(x, y, max_threat);
        last_action = q_table.chooseActionSentinela(state);
        return last_action;
    }
    
    std::vector<float> issueAlerts() const {
        std::vector<float> alerts(threat_readings.size(), 0.0f);
        for (size_t i = 0; i < threat_readings.size(); ++i) {
            if (threat_readings[i] > ALERT_THRESHOLD) {
                alerts[i] = threat_readings[i];
            }
        }
        return alerts;
    }
    
    void learn(const std::tuple<float, float, float>& state, int action, float reward, 
               const std::tuple<float, float, float>& new_state) {
        q_table.learn(state, action, reward, new_state);
    }
    
    std::tuple<float, float, float> getCurrentState() const {
        float max_threat = *max_element(threat_readings.begin(), threat_readings.end());
        return std::make_tuple(x, y, max_threat);
    }
    
    void displayStatus() const {
        std::cout << "Sentinel " << (is_dominant ? "(Dominant)" : "") 
                 << " (" << x << ", " << y << ") - "
                 << "Action: " << last_action << " Threats: [";
        for (float threat : threat_readings) {
            std::cout << threat << " ";
        }
        std::cout << "]" << std::endl;
    }

    //json conde
    json toJson() const {
        return json{
            {"type", "sentinel"},
            {"ID", id},
            {"position", {x, y}},
            {"action", last_action},
            {"threats_detected", threat_readings}
        };
    }
      
};
#endif