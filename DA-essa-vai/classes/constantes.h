#ifndef CONSTANTES_H
#define CONSTANTES_H

const int FORRAGEAR = 1;
const int PERDER_FOME = 2;
const int PASSO = 1;
const int ACERTOS_DOMINANTE = 20;
const int DISTANCIA_ATAQUE = 10;
const int RECOMPENSA_POR_FORRAGEAR = 2;
const int SACIEZ = 20;

const int QTD_INDIV = 10;
const int QTD_AMEACAS = 2;


//novo:
float LEARNING_RATE_N = 0.3f;
float DISCOUNT_FACTOR_N = 0.7f;
float EXPLORATION_RATE_N = 0.4f;
float FOOD_REWARD_N = 2.0f;

//adulto
float LEARNING_RATE_A = 0.1f;
float DISCOUNT_FACTOR_A = 0.9f;
float EXPLORATION_RATE_A = 0.1f;
float FOOD_REWARD_A = 1.5f;

//idoso
float LEARNING_RATE_I = 0.06f;
float DISCOUNT_FACTOR_I = 0.9f;
float EXPLORATION_RATE_I = 0.01f;
float FOOD_REWARD_I = 1.0f;

#endif