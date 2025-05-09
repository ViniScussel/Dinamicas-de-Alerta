#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <random>
/*
It defines a class with just one function to genrate
random numbers between 0 and 1

It uses Random.h and random.cpp
*/

using namespace std;

class Random
{
public:
    float getRand(){
        random_device rd;
        uniform_real_distribution<float> dist(0, 1);
        return dist(rd);
    }
};

#endif