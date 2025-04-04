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

#include "Individuos.h"

using namespace std;

class Ameaca : public Individuos {
    int intensidade;
public:
    Ameaca() : intensidade(1 + rand() % 4) {}
    
    int Ia() const { return intensidade; }
    
    void mover() {
        x += (rand() / float(RAND_MAX) * 0.1f - 0.05f);
        y += (rand() / float(RAND_MAX) * 0.1f - 0.05f);
        x = max(0.0f, min(1.0f, x));
        y = max(0.0f, min(1.0f, y));
    }
    
    void mostrarEstado() const {
        cout << "Ameaça (" << fixed << setprecision(2) << x << ", " << y << ") - ";
        cout << "Intensidade: " << intensidade << endl;
    }
};

#endif