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
    float randomicoEntre(int a, int b){
        random_device rd;
        uniform_int_distribution<int> dist(a, b);
        return dist(rd);
    }
    float randomicoNoMapa(){
        random_device rd;
        uniform_int_distribution<int> dist(0, 100);
        return dist(rd);
    }
};

#endif