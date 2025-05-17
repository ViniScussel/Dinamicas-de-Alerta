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

#include "extras/json.hpp"
#include "extras/Ambiente.h"
#include "extras/Ameaca.h"
#include "extras/Forrageadores.h"
#include "extras/Individuos.h"
#include "extras/QLearning.h"
#include "extras/Sentinelas.h"
#include "extras/constantes.h"

using json = nlohmann::json;
using namespace std;

/*
Adicionar:
fazer com que a tabela Q dependa da diferença de pos entre indv e ameaça e não de x e y de indv

calcular tempo de resposta dos forrageadores

ataque depende do nível de ameaça [feito]

tirar forrageadores do escopo quando morrem [feito]
adicionar vetores de forrageadores mortos e seus IDs [mais ou menos]

percepção de ameaças dinâmicas [mais ou menos, ainda tenho que arrumar]

Cada vez que um forrageador morre, todos os forrageadores são penalizados com -5
e os sentinelas com -15
o que implica em adicionar sistema de "dias"

lógica de propagação entre alertas de outros forrageadores
-> adicionar ruido de propaação menor

lógica de correção de dominância [feito]

mudar a lógica de recepção dos forrageadores [feito]

achar um jeito de trocarem de classe

exportar a tabela Q para salvar o aprendizado

OBS: refazer o código de forma estática para estados extremos
*/

int main() {
    srand(time(0));
    Environment env;
    env.populate(FORRAGER_NUMBER, SENTINEL_NUMBER, THREAT_NUMBER, FOOD_NUMBER);
    env.runSimulation();
    
    return 0;
}
