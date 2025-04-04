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

#include "Individuos.h"
#include "QLearning.h"

using namespace std;

const int SIMULATION_STEPS = 20;
const float UMBRAL_ALERTA = 2.5f;
const float LEARNING_RATE = 0.1f;
const float DISCOUNT_FACTOR = 0.9f;
const float EXPLORATION_RATE = 0.3f;

class Forrageadores : public Individuos {
    vector<float> niveis_ameaca;
    QLearningTable q_table;
    int ultima_resposta = 0;
    
public:
    Forrageadores() : q_table(LEARNING_RATE, DISCOUNT_FACTOR, EXPLORATION_RATE) {
        niveis_ameaca.resize(4, 0.0f);
    }
    
    void atualizarAmeacas(const vector<float>& ameacas_percebidas) {
        for (size_t i = 0; i < min(ameacas_percebidas.size(), niveis_ameaca.size()); ++i) {
            niveis_ameaca[i] = ameacas_percebidas[i];
        }
    }
    
    float Tr() const {
        float max_ameaca = *max_element(niveis_ameaca.begin(), niveis_ameaca.end());
        return 1.0f / (1.0f + max_ameaca); // Tempo menor para ameaças maiores
    }
    
    int Rf() {
        float max_ameaca = *max_element(niveis_ameaca.begin(), niveis_ameaca.end());
        State estado = make_tuple(x, y, max_ameaca);
        ultima_resposta = q_table.chooseAction(estado);
        return q_table.chooseAction(estado);
    }
    
    void aprender(const State& estado, int acao, float recompensa, const State& novo_estado) {
        q_table.learn(estado, acao, recompensa, novo_estado);
    }
    
    State getCurrentState() const {
        float max_ameaca = *max_element(niveis_ameaca.begin(), niveis_ameaca.end());
        return make_tuple(x, y, max_ameaca);
    }
    
    void mostrarEstado() const {
        cout << "Forrageador (" << fixed << setprecision(2) << x << ", " << y << ") - ";
        cout << "Niveis ameaca: [";
        for (float ameaca : niveis_ameaca) {
            cout << fixed << setprecision(2) << ameaca << " ";
        }
        cout << "] Tr: " << Tr() << " Rf: " << ultima_resposta << endl;
    }
};

#endif