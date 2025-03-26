#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <chrono>

#include "../../NumAleat/Random.h"
#include "Forrageadores.h"

using namespace std;

//Definir o que é a resposta certa
int Forrageadores::Rf(float precisao, int rightresponse){
    Random avarage;
    if(precisao > avarage.get_Rand_Between_One()){
        return rightresponse;
    } else {
        return avarage.get_Int_Number_Between(1, 4);
    }
}