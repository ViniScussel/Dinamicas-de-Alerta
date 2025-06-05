#ifndef INDIVIDUOS_H
#define INDIVIDUOS_H

#include <iostream>

#include "aleatorio.h"
#include "constantes.h"
#include "tabelaqfor.h"
#include "tabelaqsenti.h"

constexpr size_t NUM_EXTRA_INTEGERS = QTD_INDIV + 1 + 3;
//constexpr int TAMANHO_ESTADO = QTD_AMEACAS;

//mudar fatores q table pela idade
//idade impacta na defesa
//adicionar idade

class Individuo {
protected:
    int x;
    int y;
    bool vivo;

    Random al;

public:

    Individuo() : vivo(true) {}

    vector<int> retornarPosição() const {
        return {x, y};
    }

    bool estaVivo() const {
        return vivo;
    }

    void matar() {
        vivo = false;
    }

    void mudarPosicao(int novaPosX, int novaPosY){
        x = novaPosX;
        y = novaPosY;
    }

};

class ForSenti : public Individuo {
private:

    QLearningTableForrageador qtablefor;
    QLearningTableSentinela qtablesenti;

protected:
    int idade; // (1) novos, (2) adultos, (3) idosos 
    bool sentinelas;
    int forca_de_ataque;
//forrageadores
    int qtd_comida;
    int min_dist_A_inten;
    vector<int> min_dist_A_cord;
//tabela q
    float lr, df, er = LEARNING_RATE_A, DISCOUNT_FACTOR_A, EXPLORATION_RATE_A;
    int ultima_acao;
//sentinelas
    int max_A_intensity;
    bool dominancia;

public:
    vector<int> ameacas_percebidas;
    vector<int> ameacas_ouvidas;
                                //maior população adulta
    ForSenti() : Individuo(), idade(al.randomicoEntre(1, 4)), 
    qtablefor(lr, df, er),
    qtablesenti(lr, df, er)
    {
        switch (idade)
        {
        case 1:
            lr, df, er = LEARNING_RATE_N, DISCOUNT_FACTOR_N, EXPLORATION_RATE_N;
            break;

        case 2:
            lr, df, er = LEARNING_RATE_A, DISCOUNT_FACTOR_A, EXPLORATION_RATE_A;
            break;
        case 3:
            lr, df, er = LEARNING_RATE_A, DISCOUNT_FACTOR_A, EXPLORATION_RATE_A;
            break;

        case 4:
            lr, df, er = LEARNING_RATE_I, DISCOUNT_FACTOR_I, EXPLORATION_RATE_I;
            break;
        default:
            break;
        }

        if(al.randomicoEntre(1, 100) <= 12){
            sentinelas = true;
            int posx = al.randomicoEntre(1, 100);
            while(posx<=70 && posx>=30){posx = al.randomicoEntre(1, 100);}
            int posy = al.randomicoEntre(1, 100);
            while(posy<=70 && posy>=30){posy = al.randomicoEntre(1, 100);}
            mudarPosicao(posx, posy);
        } else {
            mudarPosicao(al.randomicoEntre(40, 80),al.randomicoEntre(30, 70));
            sentinelas = false;
        }

        if(al.randomicoEntre(1, 10)<3){dominancia=true;}
        ameacas_percebidas.resize(QTD_AMEACAS, 0);
        ameacas_ouvidas.clear();  // Initialize empty
        min_dist_A_cord.clear();  // Initialize empty
        min_dist_A_cord.resize(2, 0);
        qtd_comida = 0;           // Initialize to 0
    }

    void tamanhoDeAmeacasOuvidas(int qtd){
        ameacas_ouvidas.resize(qtd, 0);
    }

    void atualizarAmeacasOuvidas(int intensidade, int index){
        ameacas_ouvidas[index] = intensidade;
    }

    void atualizarMenorDistAEIntensidadeECord(vector<int> cord, int intensidade){
        min_dist_A_cord = cord;
        min_dist_A_inten = intensidade;
    }

    void escolherAcaoFor() {
        //erro pode estar aqui
        array<int, NUM_EXTRA_INTEGERS> state = {};
        for(int t=0; t<ameacas_ouvidas.size(); t++){
            state[t] = ameacas_ouvidas[t];
        } for(int t = ameacas_ouvidas.size(); t<ameacas_ouvidas.size()+1; t++){
            state[t] = min_dist_A_cord[t];
        }

        state[ameacas_ouvidas.size()+2] = qtd_comida;
        ultima_acao = qtablefor.chooseAction(state);
    }

    int escolherAcaoSenti() {
        array<int, TAMANHO_ESTADO> state = {};
        for(int t=0; t<ameacas_percebidas.size(); t++){
            state[t] = ameacas_percebidas[t];
        }
        ultima_acao = qtablesenti.chooseAction(state);
        return ultima_acao;
    }

    int retornarUltimaAcao(){
        return ultima_acao;
    }

    void mudarUltimaAcaoManualemte(int acao_para_mudar){
        ultima_acao = acao_para_mudar;
    }

    void aprenderFor(const array<int, NUM_EXTRA_INTEGERS>& estado, int acao, float recompensa,
        const array<int, NUM_EXTRA_INTEGERS>& novo_estado) {
        qtablefor.learn(estado, acao, recompensa, novo_estado);
    }

    void aprenderSenti(const array<int, TAMANHO_ESTADO>& estado, int acao, float recompensa, 
        const array<int, TAMANHO_ESTADO>& novo_estado){
        qtablesenti.learn(estado, acao, recompensa, novo_estado);
    }

    bool retornarDominancia(){
        return dominancia;
    }

    void executeAction() {
        switch (ultima_acao) {
            case 1: // Fugir
                /* percebe a ameaça mais perto e foge para
                na direção do vetor contrário ao vetor que
                aponta para a ameaça*/
                escapar(min_dist_A_cord);
                break;
            case 2: // Alertar
                /*age como um sentinela*/
                break;
            case 3: // Buscar abrigo
                moverAleatoriamente();
                break;
            case 4: // Vigilância
                /*atualiza a ultima casa de ameaças ouvidas
                para a ameaça mais prox dele [a fazer]*/
                vigilancia(min_dist_A_inten);
                break;
            case 5: // Forragear
                forragear();
                break;
        }
    }

    void moverAleatoriamente(){
        x += al.randomicoEntre(-1, 1) * PASSO * 2;
        y += al.randomicoEntre(-1, 1) * PASSO * 2;
    }

    void forragear(){
        qtd_comida += FORRAGEAR;
    }

    int retornarComida() const {
        return qtd_comida;
    }

    int forcaDeAtaque(){
        switch (idade)
        {
        case 1:
            forca_de_ataque =  al.randomicoEntre(1, 2);
            return forca_de_ataque;
            break;
        
        case 2:
            forca_de_ataque = al.randomicoEntre(3, 5);
            return forca_de_ataque;
            break;
        
        case 3:
            forca_de_ataque = al.randomicoEntre(3, 5);
            return forca_de_ataque;
            break;

        case 4:
            forca_de_ataque = al.randomicoEntre(2, 3);
            return forca_de_ataque;
            break;
    
        default:
            break;
        }
    }

    void vigilancia(int intensidade){
        ameacas_ouvidas[ameacas_ouvidas.size()-1] = intensidade; 
    }

    void escapar(vector<int> posicao) {
        if(x-posicao[0]<=0){
            x-=PASSO;
        } else {
            x+=PASSO;
        }
        if(y-posicao[1]<=0){
            y-=PASSO;
        } else {
            y+=PASSO;
        }
    }

    array<int, STATE_SIZE> retornarEstadoFor(){
        array<int, STATE_SIZE> estado;
        for(int i=0; i<ameacas_ouvidas.size(); i++){
            estado[i] = ameacas_ouvidas[i];
        }
        estado[ameacas_ouvidas.size()] = min_dist_A_cord[0];
        estado[ameacas_ouvidas.size()+1] = min_dist_A_cord[1];
        estado[ameacas_ouvidas.size()+2] = qtd_comida;

        return estado;
    }

    array<int, TAMANHO_ESTADO> retornarEstadoSenti(){
        array<int, TAMANHO_ESTADO> estado;
        for(int i=0; i<ameacas_percebidas.size(); i++){
            estado[i] = ameacas_percebidas[i];
        }
        return estado;
    }

    //parte sentinela
    void retirarComida(){
        qtd_comida -= PERDER_FOME;
    }

    bool retornarSentinela(){
        return sentinelas;
    }

    void trocarSentinela(){
        if(sentinelas){sentinelas = false;}
        if(!sentinelas){sentinelas = true;}
    }

    void atualizarDeteccao(int intensidade_da_ameaca, int index) {
        ameacas_percebidas[index] = (intensidade_da_ameaca);
    }
    
    void atualizarMaxIntensidade(int intensidade_max){
        max_A_intensity = intensidade_max;
    }
};

class Ameaca : public Individuo {
private:
    int intensidade;
    vector<int> posicao_do_forrageador_mais_proximo;

    Random al;
public:
    Ameaca() : Individuo(), intensidade(al.randomicoEntre(1, 4)) {
        mudarPosicao(al.randomicoEntre(30, 70), al.randomicoEntre(30, 70));
    }

    void moverForrMaisProx(vector<int> posicao) {
        if(x-posicao[0]>0){
            x-=PASSO;
        } else {
            x+=PASSO;
        }
        if(y-posicao[1]>0){
            y-=PASSO;
        } else {
            y+=PASSO;
        }
    }

    int forcaDeAtaque(float distancia){
        //return round( al.randomicoEntre(1, 5) * intensidade/(distancia));
        return round( al.randomicoEntre(1, 5) * intensidade);
    }

    int retornarIntensidade() const {
        return intensidade;
    }

    void mudarForrageadorMaisProx(int px, int py){
        posicao_do_forrageador_mais_proximo[0] = px;
        posicao_do_forrageador_mais_proximo[1] = py;
    }
};

#endif 