#ifndef AMBIENTE_H
#define AMBIENTE_H

#include <vector>
#include <memory>
#include "../Individuos/Individuos.h"
#include "../Individuos/Ameaca/Ameaca.h"

class Ambiente {
private:
    std::vector<std::unique_ptr<Individuos>> individuos;
    std::vector<Ameaca> ameacas;
    //Por enquanto é um quadrado
    float tamanhoMundo = 100.0f;

public:
    // Gerenciamento de indivíduos
    void adicionarIndividuo(std::unique_ptr<Individuos> indiv);
    void removerIndividuosMortos();

    // Simulação
    /*
    POR ENQUANTO EM CONSTRUÇÃO
    void atualizar();                                     // Atualiza todos os agentes e ameaças
    void broadcastAlerta(const Alerta& alerta);           // Envia alerta a todos os forrageadores
    */

    // Lógica espacial
    float calcularDensidade(const std::vector<float>& posicao, float raio) const;
    std::vector<Individuos*> getVizinhosProximos(const std::vector<float>& posicao, float raio) const;

    // Auxiliares
    const std::vector<Ameaca>& getAmeacas() const { return ameacas; }
};

#endif