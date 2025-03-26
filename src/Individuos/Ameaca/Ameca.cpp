#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>

#include "Ameaca.h"
#include "../../NumAleat/Random.h"
#include "../Individuos.h"
#include "../forrageadores/Forrageadores.h"

int Ameaca::Tipo_De_Ameaca(){
    Random ameaca;
    return ameaca.get_Int_Number_Between(1, 4);
}