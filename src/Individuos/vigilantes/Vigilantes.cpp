#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <chrono>

#include "../../NumAleat/Random.h"
#include "Vigilantes.h"
#include "../../NumAleat/Random.h"


//rightcall será a ameaça que aparecerá, o numero dela
float Vigilantes::Ps(float precisao, int rightcall){
    Random avarage;

    if(precisao > avarage.get_Rand_Between_One()){
        //***definir o que é rightcall
        return rightcall;
    } else {
        return avarage.get_Int_Number_Between(1, 4);
    }
}