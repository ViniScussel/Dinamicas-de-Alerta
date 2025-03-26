#ifndef VIGILANTES_H
#define VIGILANTES_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>

#include "../../NumAleat/Random.h"
#include "../Individuos.h"
#include "../forrageadores/Forrageadores.h"

/*
Nv => numero de indivíduos responsáveis pela vigilância
    Ps => capacidade do vigilante emitir um sinal adequado
    (detecta a ameaça a partir de precisão aleatória, 0 e 1)

meu:
Cf => confiabilidade de um indivíduo e seu sinal
    proporção entre sinal emitido e grau de erro
Dm => status (dominancia[1] ou subordinação[0])
*/

class Vigilantes : public Individuos
{
private:
    int Cf();
    float Dm();
public:
    float Ps(float precisao, int rightcall);
};

#endif