#include <iostream>
#include <random>
#include "Random.h"

/*
Generates a real number between 0 and 1
Used in main.cpp to generate coordenades and
other stuff that need random numbers

Uses Random.h and random.cpp
*/

using namespace std;

float Random::getRand(){
    random_device rd;
    uniform_real_distribution<double> dist(0, 1);
    return dist(rd);
}