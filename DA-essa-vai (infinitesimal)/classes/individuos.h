#ifndef INDIVIDUOS_H
#define INDIVIDUOS_H

#include <iostream>

#include "aleatorio.h"
#include "constantes.h"
#include "tabelaqfor.h"
#include "tabelaqsenti.h"

constexpr size_t NUM_EXTRA_INTEGERS = QTD_AMEACAS + 3;  // QTD_AMEACAS (ameaças ouvidas) + 2 coordenadas + comida
constexpr int TAMANHO_ESTADO = QTD_AMEACAS;         // Apenas ameaças percebidas para sentinelas
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
    QLearningTableSenti qtablesenti;
    

protected:
    int idade; // (1) novos, (2) adultos, (3) idosos 
    bool sentinelas;
    bool esta_abrigado = false;
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
            //Posição de sentinelas
            sentinelas = true;
            int borda_x = (al.randomicoEntre(0, 1) == 0)
              ? al.randomicoEntre(0, round(TAMANHO_MAPA * 0.1))
              : al.randomicoEntre(round(TAMANHO_MAPA * 0.9), TAMANHO_MAPA);
            int borda_y = (al.randomicoEntre(0, 1) == 0)
              ? al.randomicoEntre(0, round(TAMANHO_MAPA * 0.1))
              : al.randomicoEntre(round(TAMANHO_MAPA * 0.9), TAMANHO_MAPA);
            mudarPosicao(borda_x, borda_y);

        } else {
            //Posição de Forrageadores
            int px = al.randomicoEntre(round(TAMANHO_MAPA * 0.1), round(TAMANHO_MAPA * 0.9));
            int py = al.randomicoEntre(round(TAMANHO_MAPA * 0.1), round(TAMANHO_MAPA * 0.9));
            mudarPosicao(px, py);

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
        if(calcularDist(cord, retornarPosição())<=1000*al.randomicoRealEntre(1, 2)){
            min_dist_A_cord = cord;
            min_dist_A_inten = intensidade;
        } else {
            min_dist_A_cord = {-1, -1};
            min_dist_A_inten = 0;
        }
    }

    vector<int> retornarMinDistACord(){
        return min_dist_A_cord;
    }

   void escolherAcaoFor() {
        StateFor state = {};
        // Preenche ameaças ouvidas
        for(int t = 0; t < ameacas_ouvidas.size(); t++) {
            state[t] = ameacas_ouvidas[t];
        }
        // Preenche coordenadas da ameaça mais próxima
        state[QTD_AMEACAS] = min_dist_A_cord[0];
        state[QTD_AMEACAS + 1] = min_dist_A_cord[1];
        // dade de comida
        state[QTD_AMEACAS + 2] = qtd_comida;
        
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

    void aprenderFor(const StateFor& estado, int acao, float recompensa,
                    const StateFor& novo_estado) {
        qtablefor.learn(estado, acao, recompensa, novo_estado);
    }

    void aprenderSenti(const array<int, TAMANHO_ESTADO>& estado, int acao, float recompensa, 
        const array<int, TAMANHO_ESTADO>& novo_estado){
        qtablesenti.learn(estado, acao, recompensa, novo_estado);
    }

    int retornarIdade(){return idade;}

    float calcularDist(vector<int> p1, vector<int> p2) {
            int dx = abs(p1[0] - p2[0]);
            int dy = abs(p1[1] - p2[1]);
            
            // Considerar wrap-around nas bordas
            dx = min(dx, TAMANHO_MAPA - dx);
            dy = min(dy, TAMANHO_MAPA - dy);
            
            return sqrt(dx*dx + dy*dy);
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
                if(calcularDist({x, y}, min_dist_A_cord)<=500){
                    escapar(min_dist_A_cord);   
                } else if (calcularDist({x, y}, min_dist_A_cord)<al.randomicoEntre(1, TAMANHO_MAPA)){
                    escapar(min_dist_A_cord);
                } else {moverAleatoriamente();}

                qtd_comida-=1;
                esta_abrigado = false;
                break;
            case 2: // Alertar
                /*age como um sentinela*/
                esta_abrigado = false;
                break;
            case 3: // Buscar abrigo
                buscarAbrigo();
                break;
            case 4: // Vigilância
                /*atualiza a ultima casa de ameaças ouvidas
                para a ameaça mais prox dele [a fazer]*/
                esta_abrigado = false;
                vigilancia(min_dist_A_inten);
                break;
            case 5: // Forragear
                esta_abrigado = false;
                if((x>=TAMANHO_MAPA*0.1 && x<=TAMANHO_MAPA-TAMANHO_MAPA*0.1) && ((y>=TAMANHO_MAPA*0.1 && y<=TAMANHO_MAPA-TAMANHO_MAPA*0.1))){forragear();}
                break;
        }
    }

    void moverAleatoriamente() {
        // Movimento normal com wrap-around
        x += al.randomicoEntre(-PASSO, PASSO);
        y += al.randomicoEntre(-PASSO, PASSO);
        
        // Aplica wrap-around garantido
        x = (x + TAMANHO_MAPA) % TAMANHO_MAPA;
        y = (y + TAMANHO_MAPA) % TAMANHO_MAPA;
    }

    void buscarAbrigo(){
        if(al.randomicoEntre(1, 100)<=CHANCES_DE_ABRIGO){
            moverAleatoriamente();
            if(qtd_comida>=-8){qtd_comida-=2;}
            else {qtd_comida=-10;}
            esta_abrigado = true;
        }
    }

    void mudarComidaManualmente(int min){
        qtd_comida = min;
    }

    bool retornarAbrigado(){return esta_abrigado;}

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
            return 1;
            break;
        }
    }

    void vigilancia(int intensidade){
        ameacas_ouvidas[ameacas_ouvidas.size()-1] = intensidade; 
    }

    void escapar(const vector<int>& posicao) {
        if(posicao[0]==-1 || posicao[1]==-1){moverAleatoriamente();}
        else if(calcularDist(min_dist_A_cord, {x, y})>=500*al.randomicoRealEntre(1, 2)){moverAleatoriamente();}
        else{
            // Calcula direção inicial
            float dx = static_cast<float>(x - posicao[0]);
            float dy = static_cast<float>(y - posicao[1]);

            // Ajusta para menor caminho toroidal em X
            if (dx > TAMANHO_MAPA/2) {
                dx = dx - TAMANHO_MAPA;
            } else if (dx < -TAMANHO_MAPA/2) {
                dx = dx + TAMANHO_MAPA;
            }
            
            // Ajusta para menor caminho toroidal em Y
            if (dy > TAMANHO_MAPA/2) {
                dy = dy - TAMANHO_MAPA;
            } else if (dy < -TAMANHO_MAPA/2) {
                dy = dy + TAMANHO_MAPA;
            }

            // Adiciona aleatoriedade ao movimento
            dx += al.randomicoRealEntre(-0.3f, 0.3f);
            dy += al.randomicoRealEntre(-0.3f, 0.3f);

            // Normaliza o vetor de direção
            float magnitude = sqrt(dx*dx + dy*dy);
            if (magnitude > 0) {
                dx /= magnitude;
                dy /= magnitude;
            }

            // Aplica movimento (com velocidade dobrada para fugas)
            x += static_cast<int>(dx * PASSO * 2);
            y += static_cast<int>(dy * PASSO * 2);

            // Tratamento de bordas com wrap-around
            x = (x + TAMANHO_MAPA) % TAMANHO_MAPA;
            y = (y + TAMANHO_MAPA) % TAMANHO_MAPA;

            // Consome energia
            qtd_comida -= 1;
            esta_abrigado = false;
        }
    }

    bool estaAbrigado(){return esta_abrigado;}

    StateSenti retornarEstadoSenti() {
        StateSenti estado;
        for(int i = 0; i < QTD_AMEACAS; i++) {
            estado[i] = ameacas_percebidas[i];
        }
        return estado;
    }

    StateFor retornarEstadoFor() {
        StateFor estado;
        for(int i = 0; i < QTD_AMEACAS; i++) {
            estado[i] = ameacas_ouvidas[i];
        }
        estado[QTD_AMEACAS] = min_dist_A_cord[0];
        estado[QTD_AMEACAS + 1] = min_dist_A_cord[1];
        estado[QTD_AMEACAS + 2] = qtd_comida;
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
        else if(!sentinelas){sentinelas = true;}
    }

    void atualizarDeteccao(int intensidade_da_ameaca, int index) {
        ameacas_percebidas[index] = (intensidade_da_ameaca);
    }
    
    void atualizarMaxIntensidade(int intensidade_max){
        max_A_intensity = intensidade_max;
    }

    int retornarMacIntensidade(){
        return max_A_intensity;
    }
};

class Ameaca : public Individuo {
private:
    bool ativa;
    int timer;
    int tempo_restante;
    int intensidade;
    vector<int> posicao_do_forrageador_mais_proximo;

    Random al;
public:
    Ameaca() : Individuo(), 
            intensidade(al.randomicoEntre(1, 4)), 
            ativa(false),
            timer(0),
            posicao_do_forrageador_mais_proximo({0, 0}),
            tempo_restante(al.randomicoEntre(MIN_INTERVALO_AMEAÇA, MAX_INTERVALO_AMEAÇA)) {
        // Posição inicial fora do mapa quando inativa
        mudarPosicao(-1000, -1000);
    }

    void moverForrMaisProx(vector<int> posicao) {
        // Calcula direção com wrap-aware
        int target_x = posicao[0];
        int target_y = posicao[1];
        
        // Ajusta alvo se caminho toroidal for mais curto em X
        if (abs(x - target_x) > TAMANHO_MAPA/2) {
            if (x > target_x) {
                target_x += TAMANHO_MAPA;
            } else {
                target_x -= TAMANHO_MAPA;
            }
        }
        
        // Ajusta alvo se caminho toroidal for mais curto em Y
        if (abs(y - target_y) > TAMANHO_MAPA/2) {
            if (y > target_y) {
                target_y += TAMANHO_MAPA;
            } else {
                target_y -= TAMANHO_MAPA;
            }
        }

        // Movimento em X com ruído reduzido
        if (x - target_x > 0) {
            x -= PASSO_AM*intensidade + al.randomicoEntre(-1, 1);
        } else {
            x += PASSO_AM*intensidade + al.randomicoEntre(-1, 1);
        }

        // Movimento em Y com ruído reduzido
        if (y - target_y > 0) {
            y -= PASSO_AM*intensidade + al.randomicoEntre(-1, 1);
        } else {
            y += PASSO_AM*intensidade + al.randomicoEntre(-1, 1);
        }

        // Aplica wrap-around
        x = (x + TAMANHO_MAPA) % TAMANHO_MAPA;
        y = (y + TAMANHO_MAPA) % TAMANHO_MAPA;
    }

    int forcaDeAtaque(float distancia){
        return round((al.randomicoEntre(1, 5) * intensidade)/(distancia));
        //return round(al.randomicoEntre(1, 5) * intensidade);
    }

    int retornarIntensidade() const {
        return intensidade;
    }

    void mudarForrageadorMaisProx(int px, int py){
        posicao_do_forrageador_mais_proximo[0] = px;
        posicao_do_forrageador_mais_proximo[1] = py;
    }

    void atualizarEstado() {
        if (!ativa) {
            tempo_restante--;
            if (tempo_restante <= 0) {
                // Ativa a ameaça com duração aleatória
                ativa = true;
                timer = al.randomicoEntre(MIN_DURACAO_AMEAÇA, MAX_DURACAO_AMEAÇA);
                tempo_restante = al.randomicoEntre(MIN_INTERVALO_AMEAÇA, MAX_INTERVALO_AMEAÇA);
                
                // Posiciona em lugar aleatório do mapa
                int px = al.randomicoEntre(round(TAMANHO_MAPA * 0.1), round(TAMANHO_MAPA * 0.9));
                int py = al.randomicoEntre(round(TAMANHO_MAPA * 0.1), round(TAMANHO_MAPA * 0.9));
                mudarPosicao(px, py);
            }
        } else {
            timer--;
            if (timer <= 0) {
                // Desativa e remove do mapa
                ativa = false;
                mudarPosicao(-1000, -1000);
            }
        }
    }
    
    bool estaAtiva() const {
        return ativa;
    }
};

#endif