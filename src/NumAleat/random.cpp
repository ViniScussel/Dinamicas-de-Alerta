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

float Random::get_Rand_Between_One(){
    random_device rd;
    uniform_real_distribution<double> dist(0, 1);
    return dist(rd);
}

int Random::get_Rand_Between_One_Hundread(){
    random_device rd;
    uniform_int_distribution<double> dist(0, 101);
    return dist(rd);
}

int Random::get_Int_Number_Between(int a, int b){
    random_device rd;
    uniform_real_distribution<double> dist(a, b);
    return dist(rd);
}