#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <iostream>
#include <vector>

using namespace std;

class Individuos
{
protected:
vector<float> posicao;
bool estaVivo = true;
//talvez um ID para diferenciar ?

private:
    float precisao();
public:
    vector<float> Coordenada_Individual();
    float Distância_Entre_Individuos(vector<float> i1, vector<float> i2);
};

#endif