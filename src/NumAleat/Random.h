#ifndef RANDOM_H
#define RANDOM_H

/*
It defines a class with just one function to genrate
random numbers between 0 and 1

It uses Random.h and random.cpp
*/

class Random
{
public:
    float get_Rand_Between_One();
    int get_Rand_Between_One_Hundread();
    int get_Int_Number_Between(int a, int b);
};

#endif