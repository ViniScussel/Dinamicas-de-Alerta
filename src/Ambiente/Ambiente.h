#ifndef AMBIENTE_H
#define AMBIENTE_H

#include <vector>
#include <memory>
#include "../Individuos/Individuos.h"
#include "../Individuos/Ameaca/Ameaca.h"
#include "../Individuos/vigilantes/Vigilantes.h"
#include "../Individuos/forrageadores/Forrageadores.h"

class Ambiente {
private:

    std::vector<std::unique_ptr<Forrageadores>> forrageadores_;
    std::vector<std::unique_ptr<Vigilantes>> sentinelas_;
    std::vector<std::unique_ptr<Ameaca>> ameacas_;

    //retorna um float representando o ruido do ambiente, quanto mais longe, menor o retorno
    //quanto menor o retorno, menor a "ameaça"
    //ex: sinal env = 4, ruido = 0.5, sinal receb = 2
    //Os indivíduos Dominantes devem ter um aumento no sinal naturalmente
    float ruidoAmbiente(std::vector<float> pos1, std::vector<float> pos2);
    int ameacaVista();
    int ameacaEscutada();
    /*
    std::vector<std::unique_ptr<Individuos>> forrageadores;
    std::vector<std::unique_ptr<Individuos>> sentinelas;
    std::vector<Ameaca> ameacas;

    //Por enquanto é um quadrado
    float tamanhoMundo = 100.0f;
    */

public:

//Gerenciamento de Indivíduos:

    //o vetor refere-se ao vetor de coordenada individual
    float distEntreInd(std::vector<float> pos1, std::vector<float> pos2);

    //Adiciona indivíduos ao ambiente
    void adicionarIndividuos(int n_forrageadores, int n_sentinelas, int n_ameacas);



    /*
    // Gerenciamento de indivíduos
    float distEntreInd(Individuos& indiv1, Individuos& indiv2);
    //void adicionarIndividuo(std::unique_ptr<Individuos> indiv);
    void removerIndividuosMortos();

    // Simulação
    /*
    POR ENQUANTO EM CONSTRUÇÃO
    void atualizar();                                     // Atualiza todos os agentes e ameaças
    void broadcastAlerta(const Alerta& alerta);           // Envia alerta a todos os forrageadores

    // Lógica espacial
    float calcularDensidade(const std::vector<float>& posicao, float raio) const;
    std::vector<Individuos*> getVizinhosProximos(const std::vector<float>& posicao, float raio) const;

    // Auxiliares
    const std::vector<Ameaca>& getAmeacas() const { return ameacas; }
    */

};



#endif