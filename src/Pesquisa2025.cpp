//Bibliotecas padrão
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <chrono>

//Códigos importados
#include "NumAleat/Random.h"
#include "Individuos/Ameaca/Ameaca.h"
#include "Individuos/forrageadores/Forrageadores.h"
#include "Individuos/vigilantes/Vigilantes.h"
#include "Ambiente/Ambiente.h"

/*
Lembretes:

Ao final do código, melhorar os nomes das variáveis
e funções e colocar tudo em inglês

Oh, Deus!

Ia => intensidade de ameaça (1, 4)
Nf => numero de indivíduos forrageadores
    Tr => tempo de resposta dos forrageadores / intervalo entre o sinal e a resposta
    Rf => tipo de resposta dos forrageadores: 
        fuga, esconderijo, imobilidade, agressão ou alarme social
Nv => numero de indivíduos responsáveis pela vigilância
    Ps => capacidade do vigilante emitir um sinal adequado
M => taxa de mortalidade
Lf => tempo ou recursos perdidos pela resposta a ameaça
*/

/*
Prioridades:

Tudo de ameaças
Resposta do Ind
Função LOSS
Criar uma classe de Ambiente
    Repassar algumas funções a ela

*/

using namespace std;

int main(){
    Ambiente novaSim;
    Random rng;
    novaSim.adicionarIndividuos(rng.get_Int_Number_Between(1, 10), rng.get_Int_Number_Between(1, 4), rng.get_Int_Number_Between(1, 3));
    return 0;
}

/*
Importante definir como essa ameaça implica na resposta e 
como a resposta implica na função LOSS

Pode haver mais de uma ameaça por cenário?

depois eu faço

O que talvez funcione
Fugir -> O indivíduo "corre" literalmente (x+velocidade, y+velocidade),
    o que implica que a ameaça também possa se movimentar
Esconder -> reduz sua "presença", o que implica que a ameaça
    tenha um campo de visão
Agressão -> deve ser uma consequência de fugir ou esconder, talvez
    uma porcentagem de chances de conseguir sair da situação
    "dar certo"
Alarme Social -> avisa os outros indivíduos sobre uma ameaça
    a precisão pode variar de acordo com a distância da ameaça
    <subclasse de sentinela (?)>


Não tenho ideia de como fazer uma função LOSS
    Devo contar os segundos de forrageamento 
    ou fazer de outra forma? Preciso de outra
    forma além do tempo, algo que dê para contar
    mas que não dependa do tempo

    Suponhamos que exista algo tipo
    [o, o, o, 0, o, o, o, 0]
    onde cada o não representa nada, mas cada 0 representa
    não há necessidade de segundos
    Cada Ind carrega com ele a qtd de 0 que "pegar", e isso
    forma a função LOSS.
    Talvez funcione
*/

/*
Meu esquema:

    Uma classe para cada tipo de indivíduo: 
        Ameaça, Vigilante e Forrageador
        São subclasses de outra classe
    Cada um tem que ter um ponto espacial
    Como eu vou criar vários indivíduos se não existe um array de vetores
    Mas existe um vetor de obj, talvez funcione (obrigado deepseek)

    eu tenho que ter uma função de densidade e de distância entre indivíduos

    A distancia de um sentinela até a ameaçã implica na detecção?
        Se sim, a ameaça é um objeto como os indivíduos
        Vou externdeer ela em públic Individuos, se n for
        dps arruma
        Se ele anda e vê, então é objeto

    fazer de madrugada:
*/