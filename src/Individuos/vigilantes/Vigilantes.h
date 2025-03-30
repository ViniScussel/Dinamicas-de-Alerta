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

Dominancia => status (dominancia[1] ou subordinação[0])
*/

class Vigilantes : public Individuos
{
private:
    float Dominancia();
public:
    std::vector<int> Ps;
    /*
        retorno esperado é um inteiro
        ele passa por um ruido para retirar a precisão
        e retorna o int mais próximo
        3, 4 -> ruido neg
        1, 2 -> ruido pos
    */
};

#endif