#ifndef AMBIENTE_H
#define AMBIENTE_H


#include "individuos.h"
#include "aleatorio.h"

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath>
#include <algorithm>


// Definição da classe Ambiente que simula um ecossistema com indivíduos, sentinelas e ameaças
class Ambiente {
private:
    Random rand_n;

    std::vector<std::shared_ptr<ForSenti>> Individuos;  // Todos os indivíduos (forrageadores e sentinelas)
    std::vector<std::shared_ptr<ForSenti>> Sentinelas;   // Subconjunto de indivíduos que são sentinelas
    std::vector<std::unique_ptr<Ameaca>> Ameacas;       // Ameças no ambiente
    std::vector<float> recompensas;                     // Sistema de recompensas para aprendizado

public:
    // Calcula distância euclidiana entre dois pontos
    float calcularDist(vector<int> p1, vector<int> p2) {
        return std::sqrt(((p1[0]-p2[0])*(p1[0]-p2[1])) + ((p1[1]-p2[1])*(p1[1]-p2[1])));
    }

    // Popula o ambiente com indivíduos e ameaças
    void povoar() {
        // Cria indivíduos
        for(int qtd=0; qtd<QTD_INDIV; qtd++) {
            Individuos.push_back(std::make_unique<ForSenti>());
            if(Individuos[qtd]->retornarSentinela()) {
                Sentinelas.push_back(Individuos[qtd]);
            }
        }
        
        // Garante pelo menos uma sentinela
        if(Sentinelas.size() == 0) {
            Individuos[0]->trocarSentinela();
            Sentinelas.push_back(Individuos[0]);
        }

        // Cria ameaças e atualiza detecção nos indivíduos
        for(int qtd=0; qtd<QTD_AMEACAS; qtd++) {
            Ameacas.push_back(std::make_unique<Ameaca>());
            for(auto& indv : Individuos) {
                indv->atualizarDeteccao(Ameacas[qtd]->retornarIntensidade(), qtd);
            }
        }

        // Configura tamanho do vetor de ameaças ouvidas
        for(auto& indv : Individuos) {
            indv->tamanhoDeAmeacasOuvidas(Sentinelas.size()+1);
        }

        recompensas.resize(Individuos.size(), 0.0f);
        std::cout << "Povoado com sucesso" << endl;
    }

    // Repassa alertas das sentinelas para os forrageadores
    void repassarAlertas() {
        for(auto& indv : Individuos) {
            if(indv->estaVivo() && !indv->retornarSentinela()) {
                for(int i=0; i<Sentinelas.size(); i++) {
                    indv->atualizarAmeacasOuvidas(Sentinelas[i]->escolherAcaoSenti(), i);
                }
            }
        }
    }

    // Encontra o forrageador mais próximo de uma posição dada
    vector<int> acharMinDistForrageador(const vector<int>& posPropria) {
        if (Individuos.empty()) {
            return {0, 0, -1};  // Retorno inválido
        }
    
        int min_index = -1;
        float min_dist = numeric_limits<float>::max();
        vector<int> min_pos(2, 0);
    
        for (int f = 0; f < Individuos.size(); f++) {
            if (Individuos[f]->estaVivo() && !Individuos[f]->retornarSentinela()) {
                float dist = calcularDist(Individuos[f]->retornarPosição(), posPropria);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_pos = Individuos[f]->retornarPosição();
                    min_index = f;
                }
            }
        }
    
        return (min_index == -1) ? vector<int>{0, 0, -1} : vector<int>{min_pos[0], min_pos[1], min_index};
    }

    // Atualiza a ameaça mais próxima para cada indivíduo
    void acharMinDistAmeaca() {
        for (const auto& indv : Individuos) {
            if (indv && !indv->retornarSentinela() && !Ameacas.empty()) {
                const auto* mais_proxima = Ameacas[0].get();
                int menor_dist = calcularDist(indv->retornarPosição(), mais_proxima->retornarPosição());
                
                for (const auto& ameaca : Ameacas) {
                    const auto* ameaca_ptr = ameaca.get();
                    int dist_atual = calcularDist(indv->retornarPosição(), ameaca_ptr->retornarPosição());
                    if (dist_atual < menor_dist) {
                        menor_dist = dist_atual;
                        mais_proxima = ameaca_ptr;
                    }
                }
                
                indv->atualizarMenorDistAEIntensidadeECord(mais_proxima->retornarPosição(), mais_proxima->retornarIntensidade());
            }
        }
    }

    // Processa ações das ameaças (ataque ou movimento)
    void ameacaAtacaOuSeMove() {
        for (auto& am : Ameacas) {
            vector<int> result = acharMinDistForrageador(am->retornarPosição());
            int index = result[2];
    
            vector<int> pos = {result[0], result[1]};
            float dist = calcularDist(pos, am->retornarPosição());
            
            if (dist <= DISTANCIA_ATAQUE && rand_n.randomicoEntre(1, 100)<=50) {
                if (Individuos[index]->forcaDeAtaque() - am->forcaDeAtaque(dist) >= 0) {
                    Individuos[index]->escapar(am->retornarPosição());
                } else {
                    Individuos[index]->matar();
                    // Penaliza sentinelas e forrageadores
                    for (int i = 0; i < Individuos.size(); i++) {
                        if (Individuos[i]->retornarSentinela() && Individuos[i]->estaVivo()) {
                            recompensas[i] -= 10.0f;
                        } else if(!Individuos[i]->retornarSentinela() && Individuos[i]->estaVivo()) {
                            recompensas[i] -= 4.0f;
                        }
                    }
                }
            } else {
                am->moverForrMaisProx(pos);
                // Penaliza sentinelas por ameaça se aproximando
                for (int i = 0; i < Individuos.size(); i++) {
                    if (Individuos[i]->retornarSentinela() && Individuos[i]->estaVivo()) {
                        recompensas[i] -= 4.0f;
                    }
                }
            }
        }
    }

    // Processa ações dos forrageadores
    void forrageadorAcao() {
        acharMinDistAmeaca();
        for(int f=0; f<Individuos.size(); f++) {
            auto& forr = Individuos[f];
            if(forr->estaVivo() && !forr->retornarSentinela()) {
                forr->escolherAcaoFor();
                forr->executeAction();
                
                // Sistema de recompensas
                if(forr->retornarUltimaAcao() == 5) {
                    recompensas[f] += 2;
                }
                if(forr->retornarUltimaAcao() == 2) {
                    // Compartilha informação de ameaça
                    for(auto& indv : Individuos) {
                        indv->ameacas_ouvidas[indv->ameacas_ouvidas.size()-1] = forr->escolherAcaoSenti();
                    }
                }
            }
        }
    }

    // Executa ciclo de aprendizado por reforço
    void executarAprender() {
        for(int r=0; r<Individuos.size(); r++) {
            auto& indv = Individuos[r];
            if(indv->retornarSentinela()) {
                array estadoInitSenti = indv->retornarEstadoSenti();
                repassarAlertas();
                indv->aprenderSenti(estadoInitSenti, indv->retornarUltimaAcao(),
                    recompensas[r], indv->retornarEstadoSenti());
            } else {
                array estadoInitFor = indv->retornarEstadoFor();
                forrageadorAcao();
                indv->aprenderFor(estadoInitFor, indv->retornarUltimaAcao(),
                    recompensas[r], indv->retornarEstadoFor());
            }
        }
    }

    // Troca indivíduos entre posições de sentinela e forrageador
    void trocarDePosição() {
        size_t qtd_indv_trocar = 0;
        vector<int> indexs_for;
        vector<int> indexs_senti;

        // Identifica candidatos para troca
        for(int i=0; i<Individuos.size(); i++) {
            auto& indv = Individuos[i];
            if(indv->retornarComida() >= SACIEZ && indv->estaVivo() && !indv->retornarSentinela()) {
                qtd_indv_trocar++;
                indexs_for.push_back(i);
            }
            else if(indv->retornarSentinela() && indv->estaVivo()) {
                indexs_senti.push_back(i);
            }
        }

        // Ordena sentinelas por comida (menos comida primeiro)
        if(!indexs_senti.empty()) {
            for(int j=0; j<indexs_senti.size()-1; j++) {
                for(int i=0; i<indexs_senti.size()-1; i++) {
                    if(Individuos[indexs_senti[i]]->retornarComida() > Individuos[indexs_senti[i+1]]->retornarComida()) {
                        swap(indexs_senti[i], indexs_senti[i+1]);
                    }
                }
            }
        }

        // Realiza trocas
        if(qtd_indv_trocar != 0) {
            qtd_indv_trocar = min(qtd_indv_trocar, Sentinelas.size());
            for(int s=0; s<qtd_indv_trocar; s++) {
                // Troca posições e papéis
                vector<int> p = Individuos[indexs_for[s]]->retornarPosição();
                Individuos[indexs_for[s]]->mudarPosicao(Individuos[indexs_senti[s]]->retornarPosição()[0], 
                    Individuos[indexs_senti[s]]->retornarPosição()[1]);
                Individuos[indexs_senti[s]]->mudarPosicao(p[0], p[1]);
                Individuos[indexs_for[s]]->trocarSentinela();
                Individuos[indexs_senti[s]]->trocarSentinela();
            }
        }
    }

    // Executa um ciclo completo da simulação
    void execucao() {
        cout << "==================" << endl;
        // Mostra indivíduos vivos
        for(int i=0; i<Individuos.size(); i++) {
            if(Individuos[i]->estaVivo()) { cout << i << endl; }
        }
        cout << "==================" << endl;
        
        // Ciclo principal
        ameacaAtacaOuSeMove();
        executarAprender();
        trocarDePosição();
        
        // Sentinelas perdem comida a cada ciclo
        for(auto& senti : Individuos) {
            if(senti->retornarSentinela()) {
                senti->retirarComida();
            }
        }
    }
};
#endif

/*
#ifndef AMBIENTE_H
#define AMBIENTE_H

#include "individuos.h"
#include "aleatorio.h"

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cmath>
#include <algorithm>

class Ambiente{
private:
    std::vector<std::shared_ptr<ForSenti>> Individuos;
    std::vector<std::shared_ptr<ForSenti>> Sentinelas;
    std::vector<std::unique_ptr<Ameaca>> Ameacas;
    std::vector<float> recompensas;
public:
    float calcularDist(vector<int> p1, vector<int> p2){
        return std::sqrt(((p1[0]-p2[0])*(p1[0]-p2[1])) + ((p1[1]-p2[1])*(p1[1]-p2[1])));
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
        std::cout<<"Povoado com sucesso"<<endl;
    }

    void repassarAlertas(){
        for(auto& indv : Individuos){
            if(indv->estaVivo() && !indv->retornarSentinela()){
                for(int i=0; i<Sentinelas.size(); i++){
                    indv->atualizarAmeacasOuvidas(Sentinelas[i]->escolherAcaoSenti(), i);
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
            if (Individuos[f]->estaVivo() && !Individuos[f]->retornarSentinela()) {
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
            if (indv && !indv->retornarSentinela() && !Ameacas.empty()) {
                // Inicializa com a primeira ameaça válida
                const auto* mais_proxima = Ameacas[0].get();
                int menor_dist = calcularDist(indv->retornarPosição(), mais_proxima->retornarPosição());
                
                // Procura a ameaça mais próxima
                for (const auto& ameaca : Ameacas) {
                    const auto* ameaca_ptr = ameaca.get();
                    int dist_atual = calcularDist(indv->retornarPosição(), ameaca_ptr->retornarPosição());
                    if (dist_atual < menor_dist) {
                        menor_dist = dist_atual;
                        mais_proxima = ameaca_ptr;
                    }
                }
                
                // Atualiza o indivíduo
                indv->atualizarMenorDistAEIntensidadeECord(
                    mais_proxima->retornarPosição(),
                    mais_proxima->retornarIntensidade()
                );
            }
        }
    }

    void ameacaAtacaOuSeMove() {
        for (auto& am : Ameacas) {
            vector<int> result = acharMinDistForrageador(am->retornarPosição());
            int index = result[2];
    
            vector<int> pos = {result[0], result[1]};
            float dist = calcularDist(pos, am->retornarPosição());
            if (dist <= DISTANCIA_ATAQUE) {
                if (Individuos[index]->forcaDeAtaque() - am->forcaDeAtaque(dist) >= 0) {
                    Individuos[index]->escapar(am->retornarPosição());
                } else {
                    Individuos[index]->matar();
                    // Penaliza TODAS as sentinelas (como no seu código original)
                    for (int i = 0; i < Individuos.size(); i++) {
                        if (Individuos[i]->retornarSentinela() && Individuos[i]->estaVivo()) {
                            recompensas[i] -= 10.0f;
                        } if(!Individuos[i]->retornarSentinela() && Individuos[i]->estaVivo()){
                            recompensas[i] -= 4.0f;
                        }
                    }
                }
            } else {
                am->moverForrMaisProx(pos);
                // Penaliza TODAS as sentinelas (como no seu código original)
                for (int i = 0; i < Individuos.size(); i++) {
                    if (Individuos[i]->retornarSentinela() && Individuos[i]->estaVivo()) {
                        recompensas[i] -= 4.0f;
                    }
                }
            }
        }
    }

    void forrageadorAcao(){
        acharMinDistAmeaca();
        for(int f=0; f<Individuos.size(); f++){
            auto& forr = Individuos[f];
            if(forr->estaVivo() && !forr->retornarSentinela()){
                forr->escolherAcaoFor();
                forr->executeAction();
                if(forr->retornarUltimaAcao() == 5){
                    recompensas[f] += 2;
                }
                if(forr->retornarUltimaAcao() == 2){
                    for(auto& indv : Individuos){
                        indv->ameacas_ouvidas[indv->ameacas_ouvidas.size()-1] = forr->escolherAcaoSenti();
                    }
                }
            }
        }
    }

    void executarAprender(){
        for(int r=0; r<Individuos.size(); r++){
            auto& indv = Individuos[r];
            if(indv->retornarSentinela()){
                array estadoInitSenti = indv->retornarEstadoSenti();
                repassarAlertas();
                indv->aprenderSenti(estadoInitSenti, indv->retornarUltimaAcao(),
            recompensas[r], indv->retornarEstadoSenti());
            } else {
                array estadoInitFor = indv->retornarEstadoFor();
                forrageadorAcao();
                indv->aprenderFor(estadoInitFor, indv->retornarUltimaAcao(),
            recompensas[r], indv->retornarEstadoFor());
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
                        cout<<"passou aqui swap"<<endl;
                        swap(indexs_senti[i], indexs_senti[i+1]);
                    }
                }
            }
        }
        if(qtd_indv_trocar!=0){
            //o minimo entre a qtd de trocas e a qtd de senti é pq se não da mem leak;
            qtd_indv_trocar = min(qtd_indv_trocar, Sentinelas.size());
            for(int s=0; s<qtd_indv_trocar; s++){
                vector<int> p = Individuos[indexs_for[s]]->retornarPosição();
                Individuos[indexs_for[s]]->mudarPosicao(Individuos[indexs_senti[s]]->retornarPosição()[0], 
                    Individuos[indexs_senti[s]]->retornarPosição()[1]);
                Individuos[indexs_senti[s]]->mudarPosicao(p[0], p[1]);
                Individuos[indexs_for[s]]->trocarSentinela();
                Individuos[indexs_senti[s]]->trocarSentinela();
            }
        }
    }

    void execucao(){
        cout<<"=================="<<endl;
        for(int i=0; i<Individuos.size(); i++){
            if(Individuos[i]->estaVivo()){cout<<i<<endl;}
        }
        cout<<"=================="<<endl;
        
        ameacaAtacaOuSeMove();
        executarAprender();
        trocarDePosição();
        for(auto& senti : Individuos){
            if(senti->retornarSentinela()){
                senti->retirarComida();
            }
        }
    }
};
*/