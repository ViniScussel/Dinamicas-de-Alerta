#ifndef COMIDA_H
#define COMIDA_H

#include "json.hpp"
#include "QLearning.h"
#include "Individuos.h"
#include "Forrageadores.h"
#include "Sentinelas.h"
#include "Ameaca.h"
#include "constantes.h"

using json = nlohmann::json;
using namespace std;

class Food {
    float x, y;
    float quantity;
    bool available;
    
public:
    Food() : x(rand() / float(RAND_MAX)), y(rand() / float(RAND_MAX)), 
             quantity(0.5f + (rand() / float(RAND_MAX)) * 2.0f), available(true) {}
    
    std::vector<float> getPosition() const { return {x, y}; }
    float getQuantity() const { return quantity; }
    bool isAvailable() const { return available; }
    
    float collect(float amount) {
        if (!available) return 0.0f;
        
        float collected = std::min(amount, quantity);
        quantity -= collected;
        if (quantity <= 0.0f) {
            available = false;
        }
        return collected;
    }
    
    void displayStatus() const {
        std::cout << "Food (" << x << ", " << y << ") - "
                 << "Quantity: " << quantity << (available ? "" : " [EATEN]") << std::endl;
    }

    //json code
    json toJson() const {
        return json{
            {"type", "food"},
            {"position", {x, y}},
            {"quantity", quantity},
            {"available", isAvailable()}
        };
    }
};
#endif