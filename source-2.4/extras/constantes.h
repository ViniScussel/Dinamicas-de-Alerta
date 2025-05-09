#ifndef CONSTANTES_H
#define CONSTANTES_H

//QLEARNING CONST
const float LEARNING_RATE = 0.1f;
const float DISCOUNT_FACTOR = 0.9f;
const float EXPLORATION_RATE = 0.1f;
const float FOOD_REWARD = 1.5f;

//USAR ESSES NUMEROS MUDAR
int SIMULATION_STEPS = 100;
const float ALERT_THRESHOLD = 1.5f;
const float FORAGE_RADIUS = 0.1f;
const float MOVE_DISTANCE = 0.01f;
const int ACTIONS_ARE_DAYS = 50;
const float DOMINANT_RANGE = 0.15f;
const int SENTINEL_NUMBER = 1;
const int FORRAGER_NUMBER = 1;
const int THREAT_NUMBER = 2;
const int FOOD_NUMBER = 3;

//ainda não implementado
const float MIN_ATTACK_DISTANCE = 0.01f;
const float CERTAIN_ATTACK_DISTANCE = 0.001f;

#endif
