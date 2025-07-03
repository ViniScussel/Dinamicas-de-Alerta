#ifndef CONSTANTES_H
#define CONSTANTES_H

const int NUM_SIM = 50000;
const int FORRAGEAR = 2;
const int PERDER_FOME = 10;
const int PASSO = 2;
const int PASSO_AM = 2;
const int ACERTOS_DOMINANTE = 25;
const int DISTANCIA_ATAQUE = 10;
const int SACIEZ = 60;
const int TAMANHO_MAPA = 10000;
const int dm = (TAMANHO_MAPA * 0.1);
const int delta = 0.5;
const int CHANCES_DE_ABRIGO = 60;

const int MIN_INTERVALO_AMEAÇA = 50;   // Ciclos mínimos entre ativações
const int MAX_INTERVALO_AMEAÇA = 200;  // Ciclos máximos entre ativações
const int MIN_DURACAO_AMEAÇA = 30;     // Duração mínima de atividade
const int MAX_DURACAO_AMEAÇA = 200;     // Duração máxima de atividade
const float FORCA_REPULSAO = 0.5f;  // Intensidade da repulsão entre indivíduos
const float RAIO_REPULSAO = 20.0f;  // Distância de efeito da repulsão

const int QTD_INDIV = 10;
const int QTD_AMEACAS = 1;


//novo:
float LEARNING_RATE_N = 0.3f;
float DISCOUNT_FACTOR_N = 0.7f;
float EXPLORATION_RATE_N = 0.4f;
float FOOD_REWARD_N = 6.0f;

//adulto
float LEARNING_RATE_A = 0.1f;
float DISCOUNT_FACTOR_A = 0.9f;
float EXPLORATION_RATE_A = 0.1f;
float FOOD_REWARD_A = 5.5f;

//idoso
float LEARNING_RATE_I = 0.06f;
float DISCOUNT_FACTOR_I = 0.9f;
float EXPLORATION_RATE_I = 0.01f;
float FOOD_REWARD_I = 5.0f;
#endif