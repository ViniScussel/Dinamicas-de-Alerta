#ifndef INDIVIDUOS_H
#define INDIVIDUOS_H

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

class Individual {
protected:
    float x, y;
    bool alive;
public:
    int id;

    Individual() : x(rand() / float(RAND_MAX)), y(rand() / float(RAND_MAX)), alive(true) {}
    
    std::vector<float> getPosition() const { return {x, y}; }
    
    float distanceTo(const std::vector<float>& pos) const {
        float dx = x - pos[0];
        float dy = y - pos[1];
        return sqrt(dx*dx + dy*dy);
    }
    
    bool isAlive() const { return alive; }
    void die() { alive = false; }
    
    void moveRandomly() {
        x += (rand() / float(RAND_MAX)) * 0.1f - 0.05f;
        y += (rand() / float(RAND_MAX)) * 0.1f - 0.05f;
        x = std::max(0.0f, std::min(1.0f, x));
        y = std::max(0.0f, std::min(1.0f, y));
    }
    
    void moveTowards(const std::vector<float>& target) {
        float dx = target[0] - x;
        float dy = target[1] - y;
        float dist = sqrt(dx*dx + dy*dy);
        
        if (dist > 0) {
            x += (dx / dist) * MOVE_DISTANCE;
            y += (dy / dist) * MOVE_DISTANCE;
            x = std::max(0.0f, std::min(1.0f, x));
            y = std::max(0.0f, std::min(1.0f, y));
        }
    }

    void escape(const std::vector<float>& target) {
        float dx = target[0] - x;
        float dy = target[1] - y;
        float dist = sqrt(dx*dx + dy*dy);
        
        if (dist > 0) {
            x += (dx / dist) * MOVE_DISTANCE;
            y += (dy / dist) * MOVE_DISTANCE;
            x = -std::max(0.0f, std::min(1.0f, x));
            y = -std::max(0.0f, std::min(1.0f, y));
        }
    }
};
    

#endif
