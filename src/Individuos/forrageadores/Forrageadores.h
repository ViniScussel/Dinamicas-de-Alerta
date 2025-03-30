#ifndef FORRAGEADORES_H
#define FORRAGEADORES_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>

#include "../Individuos.h"
#include "../vigilantes/Vigilantes.h"

/*
Nf => numero de indivíduos forrageadores
    Tr => tempo de resposta dos forrageadores / intervalo entre o sinal e a resposta
    Rf => tipo de resposta dos forrageadores: 
        fuga, esconderijo, imobilidade, agressão ou alarme social
*/

class Forrageadores : public Individuos
{
private:

protected:
    
public:
    std::vector<int> escutando;
    float Tr();
    int Rf(float precisao, int rightresponse);

};


#endif