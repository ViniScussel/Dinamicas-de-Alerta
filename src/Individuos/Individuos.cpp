#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <chrono>

#include "Individuos.h"
#include "../NumAleat/Random.h"

vector<float> Individuos::Coordenada_Individual(){
    Random rng;
    return {rng.get_Rand_Between_One(), rng.get_Rand_Between_One()};
}

float Individuos::Distância_Entre_Individuos(vector<float> i, vector<float> j){
    float square_sum=0.0f;
    for(int k=0; k<i.size(); k++){
        float diff = abs(i[k] - j[k]);
        square_sum += diff * diff;
    }
    return sqrt(square_sum);
}

//***Por enquanto vai ficar no aleatório mesmo,
//***mas tem que mudar isso dps
float Individuos::precisao(){
    Random rng;
    return rng.get_Rand_Between_One();
}

/*
E se pudessemos fazer um "terreno" em três dimensões?
O terreno pode ser representado por uma função f(x, y), 
algo que represente fielmente um terreno e que seja função de x e y,
assim não precisariamos "modelar" e nem programar nada, é só uma coordenada a mais
para que fique mais fiel.
Pode ser algo tipo:
f(x, y) = 0.05xy + k
*/