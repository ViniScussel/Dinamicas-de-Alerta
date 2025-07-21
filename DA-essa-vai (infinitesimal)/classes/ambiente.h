#ifndef AMBIENTE_H
#define AMBIENTE_H

#include "individuos.h"
#include "aleatorio.h"
#include <array>

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath>
#include <algorithm>

// No início do arquivo (após os includes)
using StateFor = std::array<int, QTD_AMEACAS + 3>;
using StateSenti = std::array<int, QTD_AMEACAS>;

class Ambiente{
private:
    Random al;
    std::vector<std::shared_ptr<ForSenti>> Individuos;
    std::vector<std::shared_ptr<ForSenti>> Sentinelas;
    std::vector<std::unique_ptr<Ameaca>> Ameacas;
    std::vector<float> recompensas;
    std::vector<float> recompensas_rodada;

public:
    float calcularDist(vector<int> p1, vector<int> p2) {
        int dx = abs(p1[0] - p2[0]);
        int dy = abs(p1[1] - p2[1]);
        
        // Considerar wrap-around nas bordas
        dx = min(dx, TAMANHO_MAPA - dx);
        dy = min(dy, TAMANHO_MAPA - dy);
        
        return sqrt(dx*dx + dy*dy);
    }

    void aplicarRepulsao() {
        for (size_t i = 0; i < Individuos.size(); i++) {
            if (!Individuos[i]->estaVivo()) continue;
            
            for (size_t j = i + 1; j < Individuos.size(); j++) {
                if (!Individuos[j]->estaVivo()) continue;
                
                vector<int> pos1 = Individuos[i]->retornarPosição();
                vector<int> pos2 = Individuos[j]->retornarPosição();
                float dist = calcularDist(pos1, pos2);
                
                if (dist < RAIO_REPULSAO && dist > 0) {
                    // Calcula direção da repulsão
                    float dx = pos1[0] - pos2[0];
                    float dy = pos1[1] - pos2[1];
                    
                    // Normaliza e aplica força
                    dx /= dist;
                    dy /= dist;
                    float forca = FORCA_REPULSAO * (RAIO_REPULSAO - dist) / RAIO_REPULSAO;
                    
                    // Aplica movimento
                    Individuos[i]->mudarPosicao(
                        pos1[0] + dx * forca,
                        pos1[1] + dy * forca
                    );
                    Individuos[j]->mudarPosicao(
                        pos2[0] - dx * forca,
                        pos2[1] - dy * forca
                    );
                }
            }
        }
    }  

    void povoar(){
        for(int qtd=0; qtd<QTD_INDIV; qtd++){
            Individuos.push_back(std::make_unique<ForSenti>());
            if(Individuos[qtd]->retornarSentinela()){Sentinelas.push_back(Individuos[qtd]);}
        }
        if(Sentinelas.size() == 0){
            Individuos[0]->trocarSentinela();
            Sentinelas.push_back(Individuos[0]);
        }
        for(int qtd=0; qtd<QTD_AMEACAS; qtd++){
            Ameacas.push_back(std::make_unique<Ameaca>());
            for(auto& indv : Individuos){
                indv->atualizarDeteccao(Ameacas[qtd]->retornarIntensidade(), qtd);
            }
        }
        for(auto& indv : Individuos){indv->tamanhoDeAmeacasOuvidas(Sentinelas.size()+1);}
        recompensas.resize(Individuos.size(), 0.0f);
        recompensas_rodada.resize(Individuos.size(), 0.0f);
        std::cout<<"Povoado com sucesso"<<endl;
    }

    void repassarAlertas() {
        for(int i=0; i<Individuos.size(); i++) {
            auto& indv = Individuos[i];
            if(indv->estaVivo() && !indv->retornarSentinela()) {
                for(int s=0; s<Sentinelas.size(); s++) {
                    if(Sentinelas[s]->estaVivo()) {
                        indv->atualizarAmeacasOuvidas(Sentinelas[s]->escolherAcaoSenti(), s);
                    }
                }
                // Sistema de recompensas
                if(indv->retornarUltimaAcao() == ACAO_ALERTA) {
                    recompensas[i] += RECOMPENSA_ALERTA;
                    recompensas_rodada[i] += RECOMPENSA_ALERTA;
                }
            }
        }
    }

    vector<int> acharMinDistForrageador(const vector<int>& posPropria) {
        if (Individuos.empty()) {
            return {0, 0, -1};  // Retorno inválido
        }
    
        int min_index = -1;
        float min_dist = numeric_limits<float>::max();
        vector<int> min_pos(2, 0);
    
        for (int f = 0; f < Individuos.size(); f++) {
            if (Individuos[f]->estaVivo()) {
                float dist = calcularDist(Individuos[f]->retornarPosição(), posPropria);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_pos = Individuos[f]->retornarPosição();
                    min_index = f;
                }
            }
        }
    
        if (min_index == -1) {
            return {0, 0, -1};  // Nenhum forrageador válido encontrado
        }
        
        return {min_pos[0], min_pos[1], min_index};
    }

    void acharMinDistAmeaca() {
    for (const auto& indv : Individuos) {
            if (indv && !indv->retornarSentinela()) {
                // Encontra primeira ameaça ativa
                const Ameaca* mais_proxima = nullptr;
                float menor_dist = std::numeric_limits<float>::max();
                
                for (const auto& ameaca : Ameacas) {
                    if (ameaca->estaAtiva()) {
                        float dist = calcularDist(indv->retornarPosição(), ameaca->retornarPosição());
                        if (dist < menor_dist) {
                            menor_dist = dist;
                            mais_proxima = ameaca.get();
                        }
                    }
                }
                
                // Atualiza o indivíduo
                if (mais_proxima) {
                    indv->atualizarMenorDistAEIntensidadeECord(
                        mais_proxima->retornarPosição(),
                        mais_proxima->retornarIntensidade()
                    );
                } else {
                    indv->atualizarMenorDistAEIntensidadeECord({-1, -1}, 0);
                }
            }
        }
    }

    void ameacaAtacaOuSeMove() {
        for (size_t i = 0; i < Ameacas.size(); i++) {
            auto& am = Ameacas[i];
            
            // Atualiza estado da ameaça (ativa/inativa)
            am->atualizarEstado();
            
            // Pula ameaças inativas
            if (!am->estaAtiva()) continue;
            
            // Encontra forrageador mais próximo (considerando wrap-around)
            vector<int> result = acharMinDistForrageador(am->retornarPosição());
            int index = result[2];
            
            // Se não encontrou forrageador válido, pula
            if (index == -1) continue;
            
            auto& forrageador = Individuos[index];
            vector<int> pos_alvo = {result[0], result[1]};
            float dist = calcularDist(am->retornarPosição(), pos_alvo);
            
            // Decisão: atacar ou mover-se
            if (dist <= DISTANCIA_ATAQUE && al.randomicoEntre(1, 100) <= 70) { // 70% chance de ataque
                if (forrageador->estaAbrigado()) {
                    // Forrageador abrigado - ameaça se move aleatoriamente
                    am->moverForrMaisProx({am->retornarPosição()[0] + al.randomicoEntre(-PASSO_AM, PASSO_AM),
                                        am->retornarPosição()[1] + al.randomicoEntre(-PASSO_AM, PASSO_AM)});
                }
                else if (forrageador->forcaDeAtaque() - am->forcaDeAtaque(dist) >= 0) {
                    // Forrageador resiste ao ataque
                    forrageador->escapar(am->retornarPosição());
                    cout << "Forrageador " << index << " resistiu ao ataque da ameaça " << i << endl;
                    
                    // Recompensa por defesa bem-sucedida
                    recompensas[index] += 3.0f;
                    recompensas_rodada[index] += 3.0f;
                }
                else {
                    // Forrageador é morto
                    forrageador->matar();
                    cout << "Ameaça " << i << " matou forrageador " << index << endl;
                    
                    // Penaliza todos os indivíduos vivos
                    for (size_t j = 0; j < Individuos.size(); j++) {
                        if (Individuos[j]->estaVivo()) {
                            float penalidade = Individuos[j]->retornarSentinela() ? -15.0f : -8.0f;
                            recompensas[j] += penalidade;
                            recompensas_rodada[j] += penalidade;
                            
                            // Aprendizado imediato para sentinelas
                            if (Individuos[j]->retornarSentinela()) {
                                auto estado = Individuos[j]->retornarEstadoSenti();
                                Individuos[j]->aprenderSenti(estado, Individuos[j]->retornarUltimaAcao(), 
                                                        penalidade, estado);
                            }
                        }
                    }
                    
                    // Substitui sentinela morta
                    if (forrageador->retornarSentinela()) {
                        for (auto& ind : Individuos) {
                            if (ind->estaVivo() && !ind->retornarSentinela()) {
                                ind->trocarSentinela();
                                Sentinelas.push_back(ind);
                                break;
                            }
                        }
                    }
                }
            }
            else {
                // Movimento normal em direção ao forrageador
                am->moverForrMaisProx(pos_alvo);
            }
            
            // Atualiza detecção nos indivíduos (só para ameaças ativas)
            for (auto& indv : Individuos) {
                if (indv->estaVivo()) {
                    indv->atualizarDeteccao(am->estaAtiva() ? am->retornarIntensidade() : 0, i);
                }
            }
        }
    }

    void forrageadorAcao() {
        acharMinDistAmeaca();
        for(int f=0; f<Individuos.size(); f++) {
            auto& forr = Individuos[f];
            if(forr->estaVivo() && !forr->retornarSentinela()) {
                // Recompensa por estar perto do centro
                float dist_centro = calcularDist(forr->retornarPosição(), 
                    {TAMANHO_MAPA/2, TAMANHO_MAPA/2});
                if(dist_centro <= RAIO_CENTRO) {
                    recompensas[f] += RECOMPENSA_CENTRO;
                    recompensas_rodada[f] += RECOMPENSA_CENTRO;
                }
                
                forr->escolherAcaoFor();
                forr->executeAction();
                
                // Sistema de recompensas por ações
                if(forr->retornarUltimaAcao() == ACAO_FORRAGEAR) {
                    float recompensa = 0;
                    switch(forr->retornarIdade()) {
                        case 1: recompensa = FOOD_REWARD_N; break;
                        case 2: 
                        case 3: recompensa = FOOD_REWARD_A; break;
                        case 4: recompensa = FOOD_REWARD_I; break;
                    }
                    recompensas[f] += recompensa;
                    recompensas_rodada[f] += recompensa;
                }
            }
        }
    }

    void executarAprender() {
        for(int r=0; r<Individuos.size(); r++) {
            auto& indv = Individuos[r];
            if(indv->retornarSentinela()) {
                StateSenti estadoInitSenti = indv->retornarEstadoSenti();
                repassarAlertas();
                indv->aprenderSenti(estadoInitSenti, indv->retornarUltimaAcao(),
                    recompensas_rodada[r], indv->retornarEstadoSenti());
            } else {
                StateFor estadoInitFor = indv->retornarEstadoFor();
                forrageadorAcao();
                indv->aprenderFor(estadoInitFor, indv->retornarUltimaAcao(),
                    recompensas_rodada[r], indv->retornarEstadoFor());
            }
        }
    }

    void trocarDePosição(){
        size_t qtd_indv_trocar = 0;
        vector<int> indexs_for;
        vector<int> indexs_senti;
        //se der errado, tente com resize(0)
        indexs_senti.clear();
        indexs_for.clear();

        for(int i=0; i<Individuos.size(); i++){
            auto& indv = Individuos[i];
            if(indv->retornarComida() >= SACIEZ && indv->estaVivo() && !indv->retornarSentinela()){
                qtd_indv_trocar++;
                indexs_for.push_back(i);
            }
            else if(indv->retornarSentinela() && indv->estaVivo()){
                indexs_senti.push_back(i);
            }
        }
        //cout<<indexs_senti.size()<<endl;
        if(indexs_senti.size()!=0){
            for(int j=0; j<indexs_senti.size()-1; j++){
                for(int i=0; i<indexs_senti.size()-1; i++){
                    if(Individuos[indexs_senti[i]]->retornarComida()>Individuos[indexs_senti[i+1]]->retornarComida()){
                        swap(indexs_senti[i], indexs_senti[i+1]);
                    }
                }
            }
        }
        if(qtd_indv_trocar!=0){
            //o minimo entre a qtd de trocas e a qtd de senti é pq se não da mem leak;
            qtd_indv_trocar = min(qtd_indv_trocar, Sentinelas.size());
            for(int s=0; s<qtd_indv_trocar; s++){
                auto& forr = Individuos[indexs_for[s]];
                auto& senti = Individuos[indexs_senti[s]];
                vector<int> pos_for = forr->retornarPosição();
                vector<int> pos_senti = senti->retornarPosição();

                // ===== FORRAGEADOR: mover para a borda mais próxima =====
                int novo_x_for = pos_for[0];
                int novo_y_for = pos_for[1];
                int margem = round(TAMANHO_MAPA * 0.05);  // 5% da borda

                // Decide X
                if (pos_for[0] < TAMANHO_MAPA / 2)
                    novo_x_for = al.randomicoEntre(0, margem);  // esquerda
                else
                    novo_x_for = al.randomicoEntre(TAMANHO_MAPA - margem, TAMANHO_MAPA);  // direita

                // Decide Y
                if (pos_for[1] < TAMANHO_MAPA / 2)
                    novo_y_for = al.randomicoEntre(0, margem);  // topo
                else
                    novo_y_for = al.randomicoEntre(TAMANHO_MAPA - margem, TAMANHO_MAPA);  // base

                forr->mudarPosicao(novo_x_for, novo_y_for);

                // ===== SENTINELA: mover para o centro mais próximo =====
                int centro = TAMANHO_MAPA / 2;
                int desvio = round(TAMANHO_MAPA * 0.4); // 40% de ruído

                int novo_x_senti = al.randomicoEntre(centro - desvio, centro + desvio);
                int novo_y_senti = al.randomicoEntre(centro - desvio, centro + desvio);

                senti->mudarPosicao(novo_x_senti, novo_y_senti);

                // ===== Troca os papéis =====
                forr->trocarSentinela();
                senti->trocarSentinela();

            }
        }
    }

    void registrarMediaRecompensasCSV(int execucao_id) {
        float soma = std::accumulate(recompensas.begin(), recompensas.end(), 0.0f);
        float media = soma / recompensas.size();

        std::ofstream arquivo("recompensas.csv", std::ios::app);
        if (arquivo.is_open()) {
            arquivo << execucao_id << "," << media << "\n";
            arquivo.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo resultados.csv\n";
        }
    }

    void registrarMediaRecompensaRodadaCSV(int ex_id){
        float soma = std::accumulate(recompensas_rodada.begin(), recompensas_rodada.end(), 0.0f);
        float media = soma / recompensas_rodada.size();

        std::ofstream arquivo("recompensas_rodada.csv", std::ios::app);
        if (arquivo.is_open()) {
            arquivo << ex_id << "," << media << "\n";
            arquivo.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo resultados.csv\n";
        }
        for(int i=0; i<recompensas_rodada.size(); i++){recompensas_rodada[i]=0.0f;}
    }

    void registrarNumIndv(int ex_id){
        int qtd_indv=0;
        for(auto& indv : Individuos){if(indv->estaVivo()){qtd_indv++;}}
        std::ofstream arquivo("qtd_indv.csv", std::ios::app);
        if (arquivo.is_open()) {
            arquivo << ex_id << "," << qtd_indv << "\n";
            arquivo.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo resultados.csv\n";
        }
    }

    void registrarMovimentos(int execucao_id) {
        std::ofstream arquivo("movimentos.csv", std::ios::app);
        if (arquivo.is_open()) {
            for (int i = 0; i < Individuos.size(); i++) {
                auto& indv = Individuos[i];
                int x = indv->retornarPosição()[0];
                int y = indv->retornarPosição()[1];
                bool vivo = indv->estaVivo();
                int acao = indv->retornarUltimaAcao();
                int idade = indv->retornarIdade();
                int comida = indv->retornarComida();
                int tipo = indv->retornarSentinela() ? 1 : 0; // 1=Sentinela, 0=Forrageador

                arquivo << execucao_id << "," << i << "," << tipo << "," << vivo << ","
                        << acao << "," << idade << "," << comida << "," << x << "," << y << "\n";
            }
            arquivo.close();
        } else {
            std::cerr << "Erro ao abrir movimentos.csv\n";
        }
    }

    void registrarAmeacasCSV(int execucao_id) {
    std::ofstream arquivo("ameacas.csv", std::ios::app);
    if (arquivo.is_open()) {
        for (int i = 0; i < Ameacas.size(); i++) {
            auto& a = Ameacas[i];
            vector<int> pos = a->retornarPosição();
            int intensidade = a->retornarIntensidade();
            arquivo << execucao_id << "," << i << "," << intensidade << "," << pos[0] << "," << pos[1] << "\n";
        }
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir ameacas.csv\n";
    }
}



    void execucao(){
        static int ciclo = 0;
        ciclo++;
        for(int ex=0; ex<NUM_SIM; ex++){
            cout << "==================" << endl;
            // Mostra indivíduos vivos
            int qtd_vivo=0;
            for(int i=0; i<Individuos.size(); i++) {
                if(Individuos[i]->retornarSentinela() && Individuos[i]->estaVivo()) {cout<<"S";}
                if(Individuos[i]->estaVivo()) { cout << i << " " << Individuos[i]->retornarComida() << endl; qtd_vivo++;}
            }
            cout << "==================" << endl;
            
            aplicarRepulsao();
            ameacaAtacaOuSeMove();
            executarAprender();
            trocarDePosição();
            for(auto& senti : Individuos){
                if(senti->retornarSentinela() && senti->retornarComida()>=-10 && senti->retornarComida()-PERDER_FOME>=-10){
                    senti->retirarComida();
                    if(al.randomicoEntre(1, 100)>=50){senti->moverAleatoriamente();} //50% de chance de se mover
                } else if(senti->retornarSentinela() && senti->retornarComida()>=-10 && senti->retornarComida()-PERDER_FOME<=-10){
                    senti->mudarComidaManualmente(-10);
                }
            }

            registrarNumIndv(ex);
            registrarMediaRecompensaRodadaCSV(ex);
            registrarMovimentos(ex);
            registrarAmeacasCSV(ex);
            if(Sentinelas.size()==qtd_vivo){ cout<<"TODOS OS FORRAGEADORES MORRERAM!"; break;}
        }
    }
};

#endif
