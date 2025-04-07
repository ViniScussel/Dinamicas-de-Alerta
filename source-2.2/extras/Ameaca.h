#ifndef AMEACA_H
#define AMEACA_H

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
#include "constantes.h"

using namespace std;


class Threat : public Individual {
    int intensity;
public:
    Threat() : intensity(1 + rand() % 4) {}
    
    int getIntensity() const { return intensity; }
    
    void move() {
        x += (rand() / float(RAND_MAX)) * 0.1f - 0.05f;
        y += (rand() / float(RAND_MAX)) * 0.1f - 0.05f;
        x = std::max(0.0f, std::min(1.0f, x));
        y = std::max(0.0f, std::min(1.0f, y));
    }
    
    void displayStatus() const {
        std::cout << "Threat (" << x << ", " << y << ") - "
                 << "Intensity: " << intensity << std::endl;
    }
};


#endif