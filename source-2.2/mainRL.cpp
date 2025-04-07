#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <map>
#include <tuple>

#include "extras/Ambiente.h"
#include "extras/Ameaca.h"
#include "extras/Forrageadores.h"
#include "extras/Individuos.h"
#include "extras/QLearning.h"
#include "extras/Sentinelas.h"

using namespace std;

/*
Adicionar:

Cada vez que um forrageador morre, todos os forrageadores são penalizados com -5
e os sentinelas com -15
o que implica em adicionar sistema de "dias"

lógica de propagação entre alertas de outros forrageadores

lógica de correção de dominância

mudar a lógica de recepção dos forrageadores

achar um jeito de trocarem de classe

OBS: refazer o código de forma estática para estados extremos
*/

int main() {
    srand(time(0));
    
    Environment env;
    env.populate(1, 2, 1, 1); // foragers, sentinels, threats, food sources
    env.runSimulation();
    
    return 0;
}
