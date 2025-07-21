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

// Número de quadrantes (ex: 3x3)
constexpr int NUM_QUADRANTES = 3;
constexpr int TAM_QUADRANTE = TAMANHO_MAPA / NUM_QUADRANTES;

const int QTD_INDIV = 100000;
const int PORCENTAGEM_SENTINELA = 12;
const int QTD_AMEACAS = 1;

// Parâmetros geográficos
constexpr int RAIO_CENTRO = TAMANHO_MAPA/2;      // Raio considerado "perto do centro"
constexpr int RAIO_REPULSAO = 100;     // Raio mínimo entre indivíduos
constexpr float FORCA_REPULSAO = 10.0f; // Força de repulsão entre indivíduos

// Ações possíveis (deveriam corresponder aos números usados nos switch cases)
constexpr int ACAO_FUGIR = 1;          // Fugir de ameaça
constexpr int ACAO_ALERTA = 2;         // Enviar alerta para outros
constexpr int ACAO_ABRIGAR = 3;        // Buscar abrigo
constexpr int ACAO_MOVER_ALEATORIO = 4; // Movimento aleatório
constexpr int ACAO_FORRAGEAR = 5;      // Buscar comida

// Sistema de recompensas
constexpr float RECOMPENSA_ALERTA = 4.0f;      // Recompensa por alertar outros
constexpr float RECOMPENSA_CENTRO = 10.0f;     // Recompensa por ficar perto do centro
constexpr float PENALIDADE_AMEAQA_PROXIMA = -4.0f; // Penalidade por ameaça próxima
constexpr float RECOMPENSA_DEFESA = 3.0f;      // Recompensa por defesa bem-sucedida
constexpr float PENALIDADE_MORTE_SENTINELA = -15.0f; // Penalidade por morte de sentinela
constexpr float PENALIDADE_MORTE_FORRAGEADOR = -8.0f; // Penalidade por morte de forrageador


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