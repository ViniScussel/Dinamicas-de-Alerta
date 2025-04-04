#ifndef INDIVIDUOS_H
#define INDIVIUDOS_H

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

using namespace std;

class Individuos {
    protected:
        float x, y;
        bool vivo;
    public:
        Individuos() : x(rand() / float(RAND_MAX)), y(rand() / float(RAND_MAX)), vivo(true) {}
        
        vector<float> Coordenada_Individual() const {
            return {x, y};
        }
        
        float distancia_para(const vector<float>& pos) const {
            float dx = x - pos[0];
            float dy = y - pos[1];
            return sqrt(dx*dx + dy*dy);
        }
        
        bool estaVivo() const { return vivo; }
        void morrer() { vivo = false; }
    };
    

#endif