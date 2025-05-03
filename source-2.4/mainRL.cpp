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

using json = nlohmann::json;
using namespace std;

/*
Adicionar:

Cada vez que um forrageador morre, todos os forrageadores são penalizados com -5
e os sentinelas com -15
o que implica em adicionar sistema de "dias"

lógica de propagação entre alertas de outros forrageadores [feito]

lógica de correção de dominância [feito]

mudar a lógica de recepção dos forrageadores [feito]

achar um jeito de trocarem de classe

exportar a tabela Q para salvar o aprendizado

OBS: refazer o código de forma estática para estados extremos
*/

int main() {

    int f, s, t, c;
    cout << "qtd forrageadores" << endl;
    cin >> f;
    cout << "qtd sentinelas" << endl;
    cin >> s;
    cout << "qtd ameaças" << endl;
    cin >> t;
    cout << "qtd comida" << endl;
    cin >> c;

    srand(time(0));
    Environment env;
    env.identify(); //identifica os indivíduos com os respectivos IDs
    env.populate(f, s, t, c); // 4 foragers, 2 sentinels, 3 threats, 8 food sources
    env.runSimulation();
    
    return 0;
}
