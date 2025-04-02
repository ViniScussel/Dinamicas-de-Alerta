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

using namespace std;

//Eu pedi pro ChatGPT comentar o código e me dizer se os nomes que eu usei estão condizentes para mudar
//Também pedi para ele gerar uma imagem fazendo a relação entre passos e recompensas até eu organizar
//as saídas para poder fazer os gráficos em python

// Constantes da simulação
const int SIMULATION_STEPS = 20;
const float UMBRAL_ALERTA = 2.5f;
const float LEARNING_RATE = 0.1f;   
const float DISCOUNT_FACTOR = 0.9f;
const float EXPLORATION_RATE = 0.3f;

// Tipo para representar o estado (posição x, posição y, nível de ameaça)
using State = tuple<float, float, float>;

class QLearningTable {
private:
    map<State, map<int, float>> q_table;
    float learning_rate;
    float discount_factor;
    float exploration_rate;

public:
    QLearningTable(float lr, float df, float er) 
        : learning_rate(lr), discount_factor(df), exploration_rate(er) {}

    // Escolhe uma ação baseada no estado atual (ε-greedy)
    int chooseAction(const State& state) {
        if ((rand() / float(RAND_MAX)) < exploration_rate) {
            // Exploração: ação aleatória
            return 1 + rand() % 4; // Ações de 1 a 4
        }
        
        // Exploitation: ação com maior valor Q
        auto& actions = q_table[state];
        if (actions.empty()) {
            return 1 + rand() % 4; // Ação aleatória se estado não conhecido
        }
        
        return max_element(actions.begin(), actions.end(),
            [](const pair<int, float>& a, const pair<int, float>& b) {
                return a.second < b.second;
            })->first;
    }

    // Atualiza a Q-table com a nova experiência
    void learn(const State& state, int action, float reward, const State& next_state) {
        float current_q = q_table[state][action];
        float max_next_q = 0.0f;
        
        if (!q_table[next_state].empty()) {
            max_next_q = max_element(q_table[next_state].begin(), q_table[next_state].end(),
                [](const pair<int, float>& a, const pair<int, float>& b) {
                    return a.second < b.second;
                })->second;
        }
        
        // Equação de Q-learning
        q_table[state][action] = current_q + learning_rate * 
                                (reward + discount_factor * max_next_q - current_q);
    }

    // Retorna o valor Q para um estado-ação específico (para debug)
    float getQValue(const State& state, int action) const {
        auto state_it = q_table.find(state);
        if (state_it == q_table.end()) return 0.0f;
        
        auto action_it = state_it->second.find(action);
        if (action_it == state_it->second.end()) return 0.0f;
        
        return action_it->second;
    }
};

// Classe base para todos os indivíduos (mantida igual)
class Individuos {
protected:
    float x, y;
    bool vivo;
public:
    Individuos() : x(rand() / float(RAND_MAX)), y(rand() / float(RAND_MAX)), vivo(true) {}
    
    vector<float> Coordenada_Individual() const {
        return {x, y};
    }
    
    float distancia_para(const vector<float>& pos) const {
        float dx = x - pos[0];
        float dy = y - pos[1];
        return sqrt(dx*dx + dy*dy);
    }
    
    bool estaVivo() const { return vivo; }
    void morrer() { vivo = false; }
};

// Classe Forrageadores com Q-Learning
class Forrageadores : public Individuos {
    vector<float> niveis_ameaca;
    QLearningTable q_table;
    int ultima_resposta = 0;
    
public:
    Forrageadores() : q_table(LEARNING_RATE, DISCOUNT_FACTOR, EXPLORATION_RATE) {
        niveis_ameaca.resize(4, 0.0f);
    }
    
    void atualizarAmeacas(const vector<float>& ameacas_percebidas) {
        for (size_t i = 0; i < min(ameacas_percebidas.size(), niveis_ameaca.size()); ++i) {
            niveis_ameaca[i] = ameacas_percebidas[i];
        }
    }
    
    float Tr() const {
        float max_ameaca = *max_element(niveis_ameaca.begin(), niveis_ameaca.end());
        return 1.0f / (1.0f + max_ameaca); // Tempo menor para ameaças maiores
    }
    
    int Rf() {
        float max_ameaca = *max_element(niveis_ameaca.begin(), niveis_ameaca.end());
        State estado = make_tuple(x, y, max_ameaca);
        ultima_resposta = q_table.chooseAction(estado);
        return q_table.chooseAction(estado);
    }
    
    void aprender(const State& estado, int acao, float recompensa, const State& novo_estado) {
        q_table.learn(estado, acao, recompensa, novo_estado);
    }
    
    State getCurrentState() const {
        float max_ameaca = *max_element(niveis_ameaca.begin(), niveis_ameaca.end());
        return make_tuple(x, y, max_ameaca);
    }
    
    void mostrarEstado() const {
        cout << "Forrageador (" << fixed << setprecision(2) << x << ", " << y << ") - ";
        cout << "Niveis ameaca: [";
        for (float ameaca : niveis_ameaca) {
            cout << fixed << setprecision(2) << ameaca << " ";
        }
        cout << "] Tr: " << Tr() << " Rf: " << ultima_resposta << endl;
    }
};

// Classes Sentinelas e Ameaca mantidas iguais
class Sentinelas : public Individuos {
    vector<float> Ps;
    bool dominancia;
public:
    Sentinelas() : dominancia(rand() % 2) {
        Ps.resize(4, 0.0f);
    }
    
    bool isDominante() const { return dominancia; }
    
    void atualizarAmeacas(const vector<float>& ameacas_detectadas) {
        for (size_t i = 0; i < min(ameacas_detectadas.size(), Ps.size()); ++i) {
            Ps[i] = ameacas_detectadas[i];
        }
    }
    
    vector<float> emitirAlerta() const {
        vector<float> alertas(Ps.size(), 0.0f);
        for (size_t i = 0; i < Ps.size(); ++i) {
            if (Ps[i] > UMBRAL_ALERTA) {
                alertas[i] = dominancia ? Ps[i] * 1.2f : Ps[i];
            }
        }
        return alertas;
    }
    
    void mostrarEstado() const {
        cout << "Sentinel" << (dominancia ? "a Dominante" : "a") << " (" << fixed << setprecision(2) << x << ", " << y << ") - ";
        cout << "Ameaças percebidas: [";
        for (float ameaca : Ps) {
            cout << fixed << setprecision(2) << ameaca << " ";
        }
        cout << "]" << endl;
    }
};

class Ameaca : public Individuos {
    int intensidade;
public:
    Ameaca() : intensidade(1 + rand() % 4) {}
    
    int Ia() const { return intensidade; }
    
    void mover() {
        x += (rand() / float(RAND_MAX) * 0.1f - 0.05f);
        y += (rand() / float(RAND_MAX) * 0.1f - 0.05f);
        x = max(0.0f, min(1.0f, x));
        y = max(0.0f, min(1.0f, y));
    }
    
    void mostrarEstado() const {
        cout << "Ameaça (" << fixed << setprecision(2) << x << ", " << y << ") - ";
        cout << "Intensidade: " << intensidade << endl;
    }
};

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

int main() {
    srand(time(0));
    Ambiente ambiente;
    ambiente.adicionarIndividuos(3, 2, 2);
    ambiente.simular();
    return 0;
}