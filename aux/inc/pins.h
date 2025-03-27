#ifndef H_PINS
#define H_PINS

// pin 0 - hazard lights                 Pino da luz de pisca alerta
// pin 1 - Hazard lights button          Pino do botão do pisca alerta
// pin 2 - REB activation                Pino para ativação do REB (sistema de retorno de energia)
// pin 3 - REB IPC warning               Pino para sinal de alerta do IPC (Instrument Cluster)
// pin 4 - REB deactivate                Pino para desativação do REB 
// pin 5 - Engine REB mode               Pino para controlar o modo de REB do motor
// pin 6 - REB IPC fault pin             Pino para sinal de falha do IPC no sistema REB 
// pin 7 - IPC Can Filter                Pino para ativar/destivar filtro da CAN no IPC


#define HAZARD_LIGHTS_PIN 0     
#define HAZARD_BUTTON_PIN 1         
#define REB_ACTIVATE_PIN 2
#define REB_IPC_WARNING 3
#define REB_DEACTIVATE 4
#define ENGINE_REB_MODE 5
#define REB_IPC_FAULT_PIN 6

#define CAN_FILTER_PIN 7


#endif