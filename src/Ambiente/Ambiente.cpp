#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <chrono>

#include "Ambiente.h"
#include "../Individuos/Individuos.h"

float Ambiente::distEntreInd(std::vector<float> pos1, std::vector<float> pos2){
    float dx = pos1[0] - pos2[0];
    float dy = pos1[1] - pos2[1];
    return std::sqrt(dx*dx + dy*dy);
}

void Ambiente::adicionarIndividuos(int nf, int ns, int na) {
    // Cria forrageadores
    for (int i = 0; i < nf; ++i) {
        forrageadores_.push_back(std::make_unique<Forrageadores>());
    }
    
    // Cria sentinelas
    for (int i = 0; i < ns; ++i) {
        sentinelas_.push_back(std::make_unique<Vigilantes>());
    }
    
    // Cria ameaças
    for (int i = 0; i < na; ++i) {
        ameacas_.push_back(std::make_unique<Ameaca>());
    }
}

float Ambiente::ruidoAmbiente(std::vector<float> sentinela, std::vector<float> forrageador){
    double distancia = Ambiente::distEntreInd(sentinela, forrageador);


    const double alpha = 0.0015;  // Coeficiente de absorção sonora (ar seco)
    const double h = 40.0;        // Umidade relativa (40%)
    const double h_ideal = 50.0;  // Umidade ideal (50%)
    const double T = 308.0;       // Temperatura atual (35°C em Kelvin)
    const double T_ideal = 293.0; // Temperatura ideal (20°C)
    const double p = 0.1;         // Poluição sonora (baixa)
    const double k = 0.3;         // Fator de atenuação por poluição

    // Cálculo dos fatores
    double decaimento = exp(-alpha * distancia);
    double poluicao = 1.0 / (1.0 + k * p);
    double umidade = exp(-pow(h - h_ideal, 2) / 800.0);
    double temperatura = T_ideal / T;

    // Qualidade do som (0.0 a 1.0)
    double Q = decaimento * poluicao * umidade * temperatura;
    return Q;
}


//Existe a probabilidade disso dar erro e ele não atualizar a lista constantemente
//Se isso acontecer, um retorno deve ajudar
int Ambiente::ameacaVista(){
    for(int numSenti=0; numSenti<sentinelas_.size(); numSenti++){
        for(int numAm=0; numAm<ameacas_.size(); numAm++){
            if(ameacas_[numAm]->Tipo_De_Ameaca()>2){
                sentinelas_[numSenti]->Ps[numAm] = std::round(
                ameacas_[numAm]->Tipo_De_Ameaca() -
                ruidoAmbiente(ameacas_[numAm]->Coordenada_Individual(), sentinelas_[numSenti]-> Coordenada_Individual())
                );
            } else {
                sentinelas_[numSenti]->Ps[numAm] = std::round(
                ameacas_[numAm]->Tipo_De_Ameaca() +
                ruidoAmbiente(ameacas_[numAm]->Coordenada_Individual(), sentinelas_[numSenti]-> Coordenada_Individual())
                );
            }
        }
    }
}

int Ambiente::ameacaEscutada(){
    for(int i=0; i<forrageadores_.size(); i++){
        for(int j=0; j<sentinelas_.size(); j++){
            for(int k=0; k<sentinelas_[j]->Ps.size(); k++){
                if(sentinelas_[j]->Ps[k]>2){
                    forrageadores_[i]->escutando[k] = std::round(
                        sentinelas_[j]->Ps[k] -
                        ruidoAmbiente(forrageadores_[i]->Coordenada_Individual(), sentinelas_[i]->Coordenada_Individual())
                    );
                } else {
                    forrageadores_[i]->escutando[k] = std::round(
                        sentinelas_[j]->Ps[k] +
                        ruidoAmbiente(forrageadores_[i]->Coordenada_Individual(), sentinelas_[i]->Coordenada_Individual())
                    );
                }
            }
        }       
    }
}



