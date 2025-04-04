#ifndef AMBIENTE_H
#define AMBIENTE_H
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

#include "QLearning.h"
#include "Individuos.h"
#include "Forrageadores.h"
#include "Sentinelas.h"
#include "Ameaca.h"

using namespace std;

class Ambiente {
    vector<unique_ptr<Forrageadores>> forrageadores;
    vector<unique_ptr<Sentinelas>> sentinelas;
    vector<unique_ptr<Ameaca>> ameacas;
    
    float ruidoAmbiente(const vector<float>& pos1, const vector<float>& pos2) const {
        float distancia = distEntreInd(pos1, pos2);
        if (distancia < 0.001f) return 1.0f;
        const float alpha = 0.0015f;
        const float decaimento = exp(-alpha * distancia);
        float poluicao = 0.8f + (rand() / float(RAND_MAX)) * 0.4f;
        float umidade = 0.7f + (rand() / float(RAND_MAX)) * 0.6f;
        return decaimento * poluicao * umidade;
    }
    
public:
    void adicionarIndividuos(int nf, int ns, int na) {
        for (int i = 0; i < nf; ++i) {
            forrageadores.push_back(make_unique<Forrageadores>());
        }
        for (int i = 0; i < ns; ++i) {
            sentinelas.push_back(make_unique<Sentinelas>());
        }
        for (int i = 0; i < na; ++i) {
            ameacas.push_back(make_unique<Ameaca>());
        }
    }
    
    float distEntreInd(const vector<float>& pos1, const vector<float>& pos2) const {
        float dx = pos1[0] - pos2[0];
        float dy = pos1[1] - pos2[1];
        return sqrt(dx*dx + dy*dy);
    }
    
    void detectarAmeacas() {
        for (auto& sentinela : sentinelas) {
            vector<float> ameacas_detectadas(ameacas.size(), 0.0f);
            for (size_t i = 0; i < ameacas.size(); ++i) {
                if (ameacas[i]->estaVivo()) {
                    float distancia = sentinela->distancia_para(ameacas[i]->Coordenada_Individual());
                    float qualidade = max(0.4f, ruidoAmbiente(sentinela->Coordenada_Individual(), ameacas[i]->Coordenada_Individual()));
                    ameacas_detectadas[i] = ameacas[i]->Ia() * qualidade / (1.0f + distancia);
                }
            }
            sentinela->atualizarAmeacas(ameacas_detectadas);
        }
    }
    
    void transmitirAlertas() {
        for (auto& forrageador : forrageadores) {
            vector<float> ameacas_percebidas(ameacas.size(), 0.0f);
            for (auto& sentinela : sentinelas) {
                vector<float> alertas = sentinela->emitirAlerta();
                for (size_t i = 0; i < alertas.size(); ++i) {
                    if (alertas[i] > 0) {
                        float qualidade = ruidoAmbiente(forrageador->Coordenada_Individual(), 
                                                      sentinela->Coordenada_Individual());
                        ameacas_percebidas[i] = max(ameacas_percebidas[i], alertas[i] * qualidade);
                    }
                }
            }
            forrageador->atualizarAmeacas(ameacas_percebidas);
        }
    }
    
    float calcularRecompensa(int acao, float nivel_ameaca) {
        // Recompensa baseada na adequação da ação ao nível de ameaça
        if (nivel_ameaca > 3.5f && acao == 1) return 2.0f; // Fuga para ameaças graves
        else if (nivel_ameaca > 2.5f && acao == 3) return 1.5f; // Abrigo para ameaças fortes
        else if (nivel_ameaca < 2.0f && acao == 0) return 1.0f; // Ignorar ameaças fracas
        else return -1.0f; // Penalizar ações inadequadas
    }
    
    void executarRespostas() {
        for (auto& forrageador : forrageadores) {
            State estado_atual = forrageador->getCurrentState();
            int resposta = forrageador->Rf();
            float recompensa = calcularRecompensa(resposta, get<2>(estado_atual));
            
            // Executar ação
            switch (resposta) {
                case 1: cout << "Forrageador fugiu rapidamente!" << endl; break;
                case 2: cout << "Forrageador emitiu sinal de alerta!" << endl; break;
                case 3: cout << "Forrageador procurou abrigo!" << endl; break;
                case 4: cout << "Forrageador permanece vigilante!" << endl; break;
            }
            
            // Atualizar Q-table com novo estado
            State novo_estado = forrageador->getCurrentState();
            forrageador->aprender(estado_atual, resposta, recompensa, novo_estado);
        }
    }
    
    void moverAmeacas() {
        for (auto& ameaca : ameacas) {
            ameaca->mover();
        }
    }
    
    void mostrarEstado() const {
        cout << "\n=== ESTADO DA SIMULAÇÃO ===" << endl;
        cout << "Forrageadores: " << forrageadores.size() << endl;
        cout << "Sentinelas: " << sentinelas.size() << endl;
        cout << "Ameaças: " << ameacas.size() << endl;
        
        cout << "\nDetalhes:" << endl;
        for (const auto& ameaca : ameacas) {
            if (ameaca->estaVivo()) {
                ameaca->mostrarEstado();
            }
        }
        for (const auto& sentinela : sentinelas) {
            sentinela->mostrarEstado();
        }
        for (const auto& forrageador : forrageadores) {
            forrageador->mostrarEstado();
        }
    }
    
    void passoSimulacao(int passo) {
        cout << "\n=== PASSO " << passo << " ===" << endl;
        moverAmeacas();
        detectarAmeacas();
        transmitirAlertas();
        executarRespostas();
        mostrarEstado();
    }
    
    void simular() {
        for (int i = 0; i < SIMULATION_STEPS; ++i) {
            passoSimulacao(i + 1);
        }
    }
};

#endif