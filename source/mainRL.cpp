#include "extras/QLearning.h"
#include "extras/Individuos.h"
#include "extras/Forrageadores.h"
#include "extras/Sentinelas.h"
#include "extras/Ameaca.h"
#include "extras/Ambiente.h"

//Eu pedi pro ChatGPT comentar o código e me dizer se os nomes que eu usei estão condizentes para mudar
//Também pedi para ele gerar uma imagem fazendo a relação entre passos e recompensas até eu organizar
//as saídas para poder fazer os gráficos em python

int main() {
    srand(time(0));
    Ambiente ambiente;
    ambiente.adicionarIndividuos(4, 1, 1);
    ambiente.simular();
    return 0;
}