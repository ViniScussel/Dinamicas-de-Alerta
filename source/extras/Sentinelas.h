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

#include "Individuos.h"

using namespace std;

const int SIMULATION_STEPS = 20;
const float UMBRAL_ALERTA = 2.5f;
const float LEARNING_RATE = 0.1f;
const float DISCOUNT_FACTOR = 0.9f;
const float EXPLORATION_RATE = 0.3f;


class Sentinelas : public Individuos {
    vector<float> Ps;
    bool dominancia;
public:
    Sentinelas() : dominancia(rand() % 2) {
        Ps.resize(4, 0.0f);
    }
    
    bool isDominante() const { return dominancia; }
    
    void atualizarAmeacas(const vector<float>& ameacas_detectadas) {
        for (size_t i = 0; i < min(ameacas_detectadas.size(), Ps.size()); ++i) {
            Ps[i] = ameacas_detectadas[i];
        }
    }
    
    vector<float> emitirAlerta() const {
        vector<float> alertas(Ps.size(), 0.0f);
        for (size_t i = 0; i < Ps.size(); ++i) {
            if (Ps[i] > UMBRAL_ALERTA) {
                alertas[i] = dominancia ? Ps[i] * 1.2f : Ps[i];
            }
        }
        return alertas;
    }
    
    void mostrarEstado() const {
        cout << "Sentinel" << (dominancia ? "a Dominante" : "a") << " (" << fixed << setprecision(2) << x << ", " << y << ") - ";
        cout << "Ameaças percebidas: [";
        for (float ameaca : Ps) {
            cout << fixed << setprecision(2) << ameaca << " ";
        }
        cout << "]" << endl;
    }
};
#endif