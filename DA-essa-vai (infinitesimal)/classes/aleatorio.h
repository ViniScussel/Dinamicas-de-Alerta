#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <random>

#include "constantes.h"

/*
It defines a class with just one function to genrate
random numbers between 0 and 1

It uses Random.h and random.cpp
*/

using namespace std;

class Random
{
public:
    int randomicoEntre(int a, int b){
        random_device rd;
        uniform_int_distribution<int> dist(a, b);
        return dist(rd);
    }
    int randomicoNoMapa(){
        random_device rd;
        uniform_int_distribution<int> dist(0, TAMANHO_MAPA);
        return dist(rd);
    }
    float randomicoRealEntre(float a, float b){
        random_device rd;
        uniform_real_distribution<float> dist(a, b);
        return dist(rd);
    }
    float delta(float delta){
        random_device rd;
        uniform_real_distribution<float> dist(delta, 1+delta);
        return dist(rd);
    }
};

#endif